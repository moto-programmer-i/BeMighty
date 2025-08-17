export module Vulkan:Rendering;

import <cstdint>;

import std;




// Deviceでimportされているので不要？もしくはvulkan_hppモジュールの問題かも
// import vulkan_hpp;

import :Device;
import :SwapChain;
import :Command;

namespace Vulkan {
	export class Rendering {
	public:
		Rendering(Device& device, SwapChain& swapChain, Command& command):
            // なぜかここで初期化しなければいけない
            device(device), swapChain(swapChain), command(command)
        {
			presentCompleteSemaphore = vk::raii::Semaphore(device.getDevice(), vk::SemaphoreCreateInfo());
			renderFinishedSemaphore = vk::raii::Semaphore(device.getDevice(), vk::SemaphoreCreateInfo());
			drawFence = vk::raii::Fence(device.getDevice(), { .flags = vk::FenceCreateFlagBits::eSignaled });
		}

        void drawFrame() {
            device.getQueue().waitIdle();

            auto [result, imageIndex] = swapChain.getSwapChain().acquireNextImage(
                UINT64_MAX,
                *presentCompleteSemaphore, nullptr);
            command.recordCommandBuffer(imageIndex);

            device.getDevice().resetFences(*drawFence);
            vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
            const vk::SubmitInfo submitInfo{ .waitSemaphoreCount = 1, .pWaitSemaphores = &*presentCompleteSemaphore,
                                .pWaitDstStageMask = &waitDestinationStageMask, .commandBufferCount = 1, .pCommandBuffers = &*(command.getCommandBuffer()),
                                .signalSemaphoreCount = 1, .pSignalSemaphores = &*renderFinishedSemaphore };
            device.getQueue().submit(submitInfo, *drawFence);
            while (vk::Result::eTimeout == device.getDevice().waitForFences(*drawFence, vk::True, UINT64_MAX))
                ;

            const vk::PresentInfoKHR presentInfoKHR{ .waitSemaphoreCount = 1, .pWaitSemaphores = &*renderFinishedSemaphore,
                                                    .swapchainCount = 1, .pSwapchains = &*(swapChain.getSwapChain()), .pImageIndices = &imageIndex };
            result = device.getQueue().presentKHR(presentInfoKHR);
            switch (result)
            {
            case vk::Result::eSuccess: break;
            case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n"; break;
            default: break;  // an unexpected result is returned!
            }
        }
	private:
        Device& device;
        SwapChain& swapChain;
        Command& command;
		vk::raii::Semaphore presentCompleteSemaphore = nullptr;
		vk::raii::Semaphore renderFinishedSemaphore = nullptr;
		vk::raii::Fence drawFence = nullptr;
	};
}