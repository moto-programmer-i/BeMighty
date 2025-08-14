export module Vulkan:GraphicsPipeline;

import std;



import Files;

import vulkan_hpp;

import :Device;
import :SwapChain;

// https://docs.vulkan.org/tutorial/latest/_attachments/09_shader_modules.cpp
// からコピペ

namespace Vulkan {
	export class GraphicsPipeline {
	public:
		// サンプルに合わせたが、各名前は本当は含めるべきではないかも
		GraphicsPipeline(Vulkan::Device& device, Vulkan::SwapChain& swapChain, std::string_view spvFilename, std::string_view vertName, std::string_view fragName):
			// なぜか参照の初期化はここに書かなければいけない
			device(device)
		{
			vk::raii::ShaderModule shaderModule = createShaderModule(Files::readFileToChar(spvFilename));

			vk::PipelineShaderStageCreateInfo vertShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eVertex, .module = shaderModule,  .pName = vertName.data() };
			vk::PipelineShaderStageCreateInfo fragShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eFragment, .module = shaderModule, .pName = fragName.data() };
			vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


			// 設定の詳細不明、一旦保留
			// https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/02_Fixed_functions.html

			vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
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
				.frontFace = vk::FrontFace::eClockwise,
				.depthBiasEnable = vk::False,
				.depthBiasSlopeFactor = 1.0f,
				.lineWidth = 1.0f
			};

			vk::PipelineMultisampleStateCreateInfo multisampling{
				.rasterizationSamples = vk::SampleCountFlagBits::e1,
				.sampleShadingEnable = vk::False
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

			vk::PipelineLayoutCreateInfo pipelineLayoutInfo;

			pipelineLayout = vk::raii::PipelineLayout(device.getDevice(), pipelineLayoutInfo);


			// https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/03_Render_passes.html
			vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo{
				// 上のattachmentCountと連動させる必要があるのかは不明
				.colorAttachmentCount = 1,
				.pColorAttachmentFormats = &swapChain.getSwapChainImageFormat()
			};
			vk::GraphicsPipelineCreateInfo pipelineInfo{
				.pNext = &pipelineRenderingCreateInfo,
				.stageCount = std::size(shaderStages),
				.pStages = shaderStages,
				.pVertexInputState = &vertexInputInfo, .pInputAssemblyState = &inputAssembly,
				.pViewportState = &viewportState, .pRasterizationState = &rasterizer,
				.pMultisampleState = &multisampling, .pColorBlendState = &colorBlending,
				.pDynamicState = &dynamicState, .layout = pipelineLayout, .renderPass = nullptr
			};

			graphicsPipeline = vk::raii::Pipeline(device.getDevice(), nullptr, pipelineInfo);
		}

		// nodiscard は不明
		[[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const {
			vk::ShaderModuleCreateInfo createInfo{ .codeSize = code.size() * sizeof(char), .pCode = reinterpret_cast<const uint32_t*>(code.data()) };
			vk::raii::ShaderModule shaderModule{ device.getDevice(), createInfo};

			return shaderModule;
		}

	private:
		Device& device;
		vk::raii::PipelineLayout pipelineLayout = nullptr;
		vk::raii::Pipeline graphicsPipeline = nullptr;
	};
}