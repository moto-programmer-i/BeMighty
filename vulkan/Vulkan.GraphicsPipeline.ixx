export module Vulkan:GraphicsPipeline;

import std;



import Files;

import vulkan_hpp;

import :Device;

// https://docs.vulkan.org/tutorial/latest/_attachments/09_shader_modules.cpp
// からコピペ

namespace Vulkan {
	export class GraphicsPipeline {
	public:
		// サンプルに合わせたが、各名前は本当は含めるべきではないかも
		GraphicsPipeline(Vulkan::Device& device, std::string_view spvFilename, std::string_view vertName, std::string_view fragName):
			// なぜか参照の初期化はここに書かなければいけない
			device(device)
		{
			vk::raii::ShaderModule shaderModule = createShaderModule(Files::readFileToChar(spvFilename));

			vk::PipelineShaderStageCreateInfo vertShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eVertex, .module = shaderModule,  .pName = vertName.data() };
			vk::PipelineShaderStageCreateInfo fragShaderStageInfo{ .stage = vk::ShaderStageFlagBits::eFragment, .module = shaderModule, .pName = fragName.data() };
			vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
		}

		[[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const {
			vk::ShaderModuleCreateInfo createInfo{ .codeSize = code.size() * sizeof(char), .pCode = reinterpret_cast<const uint32_t*>(code.data()) };
			vk::raii::ShaderModule shaderModule{ device.getDevice(), createInfo};

			return shaderModule;
		}

	private:
		Device& device;
	};
}