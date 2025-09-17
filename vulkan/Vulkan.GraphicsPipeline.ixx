export module Vulkan:GraphicsPipeline;

import std;



import Files;


import vulkan_hpp;

import :Device;
import :SwapChain;

import :Vertex;

// https://docs.vulkan.org/tutorial/latest/_attachments/09_shader_modules.cpp
// からコピペ

namespace Vulkan {
	export class GraphicsPipeline {
	public:
		// サンプルに合わせたが、各名前は本当は含めるべきではないかも
		GraphicsPipeline(Device& device, SwapChain& swapChain, std::string_view spvFilename, std::string_view vertName, std::string_view fragName):
			// なぜか参照の初期化はここに書かなければいけない
			device(device)
		{	
			vk::raii::ShaderModule shaderModule = createShaderModule(Files::readFileToChar(spvFilename));

			vk::PipelineShaderStageCreateInfo vertShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eVertex, .module = shaderModule,  .pName = vertName.data() };
			vk::PipelineShaderStageCreateInfo fragShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eFragment, .module = shaderModule, .pName = fragName.data() };
			vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


			// 設定の詳細不明、一旦保留
			// https://docs.vulkan.org/tutorial/latest/_attachments/18_vertex_input.cpp

			// チュートリアル用のコードが混じってしまっている
			// PipelineVertexInputStateCreateInfoを作る適切な関数を作るべきだが、
			// 今はどうやるのがベストか不明
			auto bindingDescription = Vertex::getBindingDescription();
			auto attributeDescriptions = Vertex::getAttributeDescriptions();
			vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
				.vertexBindingDescriptionCount = 1,
				.pVertexBindingDescriptions = &bindingDescription,
				.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
				.pVertexAttributeDescriptions = attributeDescriptions.data()
			};


			vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
				.topology = vk::PrimitiveTopology::eTriangleList
			};
			vk::PipelineViewportStateCreateInfo viewportState{
				.viewportCount = 1,
				.scissorCount = 1
			};

			vk::PipelineRasterizationStateCreateInfo rasterizer{
				.depthClampEnable = vk::False,
				.rasterizerDiscardEnable = vk::False,
				.polygonMode = vk::PolygonMode::eFill,
				.cullMode = vk::CullModeFlagBits::eBack,

				// 変換の都合で変えなければならなくなったらしい。どっちの方が良いかは不明
				// https://docs.vulkan.org/tutorial/latest/05_Uniform_buffers/01_Descriptor_pool_and_sets.html#_using_descriptor_sets
				// .frontFace = vk::FrontFace::eClockwise,
				 .frontFace = vk::FrontFace::eCounterClockwise,

				.depthBiasEnable = vk::False,
				// 途中でなぜかなくなった
				// https://docs.vulkan.org/tutorial/latest/_attachments/27_depth_buffering.cpp
				// .depthBiasSlopeFactor = 1.0f,
				.lineWidth = 1.0f
			};

			vk::PipelineMultisampleStateCreateInfo multisampling{
				.rasterizationSamples = vk::SampleCountFlagBits::e1,
				.sampleShadingEnable = vk::False
			};

			// https://docs.vulkan.org/tutorial/latest/_attachments/27_depth_buffering.cpp
			vk::PipelineDepthStencilStateCreateInfo depthStencil{
				.depthTestEnable = vk::True,
				.depthWriteEnable = vk::True,
				.depthCompareOp = vk::CompareOp::eLess,
				.depthBoundsTestEnable = vk::False,
				.stencilTestEnable = vk::False
			};

			vk::PipelineColorBlendAttachmentState colorBlendAttachment{
				.blendEnable = vk::False,
				.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
			};

			vk::PipelineColorBlendStateCreateInfo colorBlending{
				.logicOpEnable = vk::False,
				.logicOp = vk::LogicOp::eCopy,
				.attachmentCount = 1,
				.pAttachments = &colorBlendAttachment
			};

			std::vector dynamicStates = {
				vk::DynamicState::eViewport,
				vk::DynamicState::eScissor
			};
			vk::PipelineDynamicStateCreateInfo dynamicState{
				.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
				.pDynamicStates = dynamicStates.data()
			};

			// createDescriptorSetLayout
			std::array bindings = {
			    // 最初の引数はbindingのindex、本当はDescriptorSetLayoutを作るstatic関数が必要
			    vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr),
			    vk::DescriptorSetLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment, nullptr)
			};
			vk::DescriptorSetLayoutCreateInfo layoutInfo{ .bindingCount = static_cast<uint32_t>(bindings.size()), .pBindings = bindings.data() };
			descriptorSetLayout = vk::raii::DescriptorSetLayout(device.getDevice(), layoutInfo);

			vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
				// 本来は配列の長さ
				.setLayoutCount = 1,
				.pSetLayouts = &*descriptorSetLayout,
				.pushConstantRangeCount = 0
			};

			pipelineLayout = vk::raii::PipelineLayout(device.getDevice(), pipelineLayoutInfo);

			// https://docs.vulkan.org/tutorial/latest/_attachments/27_depth_buffering.cpp
			vk::Format depthFormat = device.findDepthFormat();

			// https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/03_Render_passes.html
			vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo{
				// 上のattachmentCountと連動させる必要があるのかは不明
				.colorAttachmentCount = 1,
				.pColorAttachmentFormats = &swapChain.getSwapChainImageFormat(),
				.depthAttachmentFormat = depthFormat
			};
			vk::GraphicsPipelineCreateInfo pipelineInfo{
				.pNext = &pipelineRenderingCreateInfo,
				.stageCount = std::size(shaderStages),
				.pStages = shaderStages,
				.pVertexInputState = &vertexInputInfo,
				.pInputAssemblyState = &inputAssembly,
				.pViewportState = &viewportState,
				.pRasterizationState = &rasterizer,
				.pMultisampleState = &multisampling,
				.pDepthStencilState = &depthStencil,
				.pColorBlendState = &colorBlending,
				.pDynamicState = &dynamicState,
				.layout = pipelineLayout,
				.renderPass = nullptr
			};

			graphicsPipeline = vk::raii::Pipeline(device.getDevice(), nullptr, pipelineInfo);

			
		}

		// nodiscard は不明
		[[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const {
			vk::ShaderModuleCreateInfo createInfo{ .codeSize = code.size() * sizeof(char), .pCode = reinterpret_cast<const uint32_t*>(code.data()) };
			vk::raii::ShaderModule shaderModule{ device.getDevice(), createInfo};

			return shaderModule;
		}

		vk::raii::DescriptorSetLayout& getDescriptorSetLayout() {
			return descriptorSetLayout;
		}

		vk::raii::PipelineLayout& getPipelineLayout() {
			return pipelineLayout;
		}


		vk::raii::Pipeline& getGraphicsPipeline() {
			return graphicsPipeline;
		}

	private:
		Device& device;
		vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
		vk::raii::PipelineLayout pipelineLayout = nullptr;
		vk::raii::Pipeline graphicsPipeline = nullptr;
	};
}