#include <vulkan/vulkan_core.h>

export module Vulkan:Command;

import std;

// Deviceでimportされているので不要？もしくはvulkan_hppモジュールの問題かも
// ここでやるとなぜか全ての補完が効かなくなる
// import vulkan_hpp;



import :Device;
import :SwapChain;
import :GraphicsPipeline;

// https://docs.vulkan.org/tutorial/latest/_attachments/14_command_buffers.cpp
// からコピペ

namespace Vulkan {
	export class Command {
	public:
		Command(Device& device, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline):
            // なぜかここで初期化しなければいけない
            swapChain(swapChain), graphicsPipeline(graphicsPipeline)
        {
			// 設定をどこまでだすのが適切か不明
			vk::CommandPoolCreateInfo poolInfo{
				.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
				.queueFamilyIndex = device.getQueueIndex()
			};
			commandPool = vk::raii::CommandPool(device.getDevice(), poolInfo);

			
			
			vk::CommandBufferAllocateInfo allocInfo{
				.commandPool = commandPool,
				.level = vk::CommandBufferLevel::ePrimary,
				// 要調査
				.commandBufferCount = 1
			};

			// なぜここだけmoveが必要なのか不明
			commandBuffer = std::move(vk::raii::CommandBuffers(device.getDevice(), allocInfo).front());
		}

        void transitionImageLayout(
            uint32_t currentFrame,
            vk::ImageLayout oldLayout,
            vk::ImageLayout newLayout,
            vk::AccessFlags2 srcAccessMask,
            vk::AccessFlags2 dstAccessMask,
            vk::PipelineStageFlags2 srcStageMask,
            vk::PipelineStageFlags2 dstStageMask
        ) {
            vk::ImageMemoryBarrier2 barrier = {
                .srcStageMask = srcStageMask,
                .srcAccessMask = srcAccessMask,
                .dstStageMask = dstStageMask,
                .dstAccessMask = dstAccessMask,
                .oldLayout = oldLayout,
                .newLayout = newLayout,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image = swapChain.getSwapChainImages()[currentFrame],

                // ほかのところにあった気がする
                .subresourceRange = {
                    .aspectMask = vk::ImageAspectFlagBits::eColor,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1
                }
            };
            vk::DependencyInfo dependencyInfo = {
                .dependencyFlags = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers = &barrier
            };
            commandBuffer.pipelineBarrier2(dependencyInfo);
        }

        vk::raii::CommandBuffer& getCommandBuffer() {
            return commandBuffer;
        }

	private:
		vk::raii::CommandPool commandPool = nullptr;
		vk::raii::CommandBuffer commandBuffer = nullptr;
        SwapChain& swapChain;
        GraphicsPipeline& graphicsPipeline;
	};
}