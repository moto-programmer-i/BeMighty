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

        /// <summary>
        /// チュートリアル用の三角形を描画するコマンドを記録
        /// </summary>
        /// <param name="imageIndex"></param>
        void recordCommandBufferToTutorial(uint32_t imageIndex) {
            commandBuffer.begin({});
            // Before starting rendering, transition the swapchain image to COLOR_ATTACHMENT_OPTIMAL
            transition_image_layout(
                imageIndex,
                vk::ImageLayout::eUndefined,
                vk::ImageLayout::eColorAttachmentOptimal,
                {},                                                     // srcAccessMask (no need to wait for previous operations)
                vk::AccessFlagBits2::eColorAttachmentWrite,                // dstAccessMask
                vk::PipelineStageFlagBits2::eTopOfPipe,                   // srcStage
                vk::PipelineStageFlagBits2::eColorAttachmentOutput        // dstStage
            );


            // この辺のマジックナンバーも要調査
            vk::ClearValue clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
            vk::RenderingAttachmentInfo attachmentInfo = {
                .imageView = swapChain.getSwapChainImageViews()[imageIndex],
                .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
                .loadOp = vk::AttachmentLoadOp::eClear,
                .storeOp = vk::AttachmentStoreOp::eStore,
                .clearValue = clearColor
            };
            vk::RenderingInfo renderingInfo = {
                .renderArea = {.offset = { 0, 0 }, .extent = swapChain.getSwapChainExtent()},
                .layerCount = 1,
                .colorAttachmentCount = 1,
                .pColorAttachments = &attachmentInfo
            };

            // 3????? 1????? 0???????
            commandBuffer.beginRendering(renderingInfo);
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline.getGraphicsPipeline());
            commandBuffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(swapChain.getSwapChainExtent().width), static_cast<float>(swapChain.getSwapChainExtent().height), 0.0f, 1.0f));
            commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), swapChain.getSwapChainExtent()));
            commandBuffer.draw(3, 1, 0, 0);
            commandBuffer.endRendering();
            // After rendering, transition the swapchain image to PRESENT_SRC
            transition_image_layout(
                imageIndex,
                vk::ImageLayout::eColorAttachmentOptimal,
                vk::ImageLayout::ePresentSrcKHR,
                vk::AccessFlagBits2::eColorAttachmentWrite,                 // srcAccessMask
                {},                                                      // dstAccessMask
                vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
                vk::PipelineStageFlagBits2::eBottomOfPipe                  // dstStage
            );
            commandBuffer.end();
        }

        void transition_image_layout(
            uint32_t currentFrame,
            vk::ImageLayout old_layout,
            vk::ImageLayout new_layout,
            vk::AccessFlags2 src_access_mask,
            vk::AccessFlags2 dst_access_mask,
            vk::PipelineStageFlags2 src_stage_mask,
            vk::PipelineStageFlags2 dst_stage_mask
        ) {
            vk::ImageMemoryBarrier2 barrier = {
                .srcStageMask = src_stage_mask,
                .srcAccessMask = src_access_mask,
                .dstStageMask = dst_stage_mask,
                .dstAccessMask = dst_access_mask,
                .oldLayout = old_layout,
                .newLayout = new_layout,
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
            vk::DependencyInfo dependency_info = {
                .dependencyFlags = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers = &barrier
            };
            commandBuffer.pipelineBarrier2(dependency_info);
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