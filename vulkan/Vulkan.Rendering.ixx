// https://docs.vulkan.org/tutorial/latest/_attachments/16_frames_in_flight.cpp
// からコピペ


// VK_QUEUE_FAMILY_IGNORED がなぜかvulkan_hppに定義されていないため
#include <vulkan/vulkan_core.h>

export module Vulkan:Rendering;

import <cstdint>;

import std;

import vulkan_hpp;


import :Settings;
import :Device;
import :SwapChain;

namespace Vulkan {
	export class Rendering {
	public:
		Rendering(Device& device, SwapChain& swapChain, const uint32_t maxFrames):
            // なぜかここで初期化しなければいけない
            device(device), swapChain(swapChain), maxFrames(maxFrames)
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
                .commandBufferCount = maxFrames
            };
            commandBuffers = vk::raii::CommandBuffers(device.getDevice(), allocInfo);
            

            presentCompleteSemaphore.clear();
            renderFinishedSemaphore.clear();
            inFlightFences.clear();
            for (size_t i = 0; i < swapChain.getSwapChainImages().size(); i++) {
                presentCompleteSemaphore.emplace_back(device.getDevice(), vk::SemaphoreCreateInfo());
                renderFinishedSemaphore.emplace_back(device.getDevice(), vk::SemaphoreCreateInfo());
            }
            for (size_t i = 0; i < maxFrames; i++) {
                inFlightFences.emplace_back(device.getDevice(), vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled });
            }
		}


        void transitionImageLayout(
            uint32_t imageIndex,
            vk::ImageLayout oldLayout,
            vk::ImageLayout newLayout,
            vk::AccessFlags2 srcAccessMask,
            vk::AccessFlags2 dstAccessMask,
            vk::PipelineStageFlags2 srcStageMask,
            vk::PipelineStageFlags2 dstStageMask
        ) {

            // ImageMemoryBarrierは、処理が順序通りに実行されるように保証
            // https://harrylovescode.gitbooks.io/vulkan-api/content/chap07/chap07.html
            vk::ImageMemoryBarrier2 barrier = {
                .srcStageMask = srcStageMask,
                .srcAccessMask = srcAccessMask,
                .dstStageMask = dstStageMask,
                .dstAccessMask = dstAccessMask,
                .oldLayout = oldLayout,
                .newLayout = newLayout,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image = swapChain.getSwapChainImages()[imageIndex],

                // ほかのところにあった気がする
                .subresourceRange = Settings::defaultImageSubresourceRange()
            };
            vk::DependencyInfo dependencyInfo = {
                .dependencyFlags = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers = &barrier
            };
            commandBuffers[currentFrame].pipelineBarrier2(dependencyInfo);
        }



        /// <summary>
        /// 1フレームを描画
        /// </summary>
        /// <param name="preReset">resetFencesなどをする直前の処理</param>
        /// <param name="recordCommandBuffer">imageIndexを使って描画するコマンド</param>
        void drawFrame(std::function<void(uint32_t)> preReset, std::function<void(uint32_t)> recordCommandBuffer) {
            // ウィンドウ最小化のときの正しい挙動が不明

            // 前のコマンドが終了するまで待つ
            while (vk::Result::eTimeout == device.getDevice().waitForFences(*inFlightFences[currentFrame], vk::True, UINT64_MAX));

            

            auto [result, imageIndex] = swapChain.getSwapChain().acquireNextImage(
                // 仕様の資料では、3番目の引数がtimeoutだが、実際は1番目
                // timeout specifies how long the function waits, in nanoseconds, if no image is available.
                // https://registry.khronos.org/vulkan/specs/latest/man/html/vkAcquireNextImageKHR.html
                UINT64_MAX,

                // セマフォ
                *presentCompleteSemaphore[semaphoreIndex],

                //  VkFence
                // セマフォのように、同期のための仕組みらしい
                // GPUが何かを完了したことをホストが知る必要がある場合にフェンスを使用
                // https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/03_Drawing/02_Rendering_and_presentation.html#_fences
                nullptr);

            // 本来はtry catchでeErrorOutOfDateKHRが発生した場合にrecreateSwapChainするべき？
            // チュートリアルではdrawの中でrecreateSwapChainを呼ぶが、
            // 今はウィンドウサイズ変更時に自動的にrecreateSwapChainを呼ぶように変更した(Window参照)
            if (result == vk::Result::eErrorOutOfDateKHR) {
                // swapChain.recreateSwapChain();
                return;
            }
            if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
                throw std::runtime_error("failed to acquire swap chain image!");
            }

            // reset前の処理
            preReset(currentFrame);

            device.getDevice().resetFences(*inFlightFences[currentFrame]);
            commandBuffers[currentFrame].reset();


            // 指定したものを描画
            recordCommandBuffer(imageIndex);


            

            // キューの送信と同期
            // パイプラインの実行段階を指定している？
            // https://docs.vulkan.org/spec/latest/_images/pipelinemesh.svg
            vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
            const vk::SubmitInfo submitInfo{
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = &*presentCompleteSemaphore[semaphoreIndex],
                .pWaitDstStageMask = &waitDestinationStageMask,

                // コマンドをバッチ実行する数
                .commandBufferCount = 1,
                .pCommandBuffers = &*commandBuffers[currentFrame],

                // 実行後のセマフォの紐づけ
                .signalSemaphoreCount = 1,
                .pSignalSemaphores = &*renderFinishedSemaphore[imageIndex]
            };
            device.getQueue().submit(submitInfo, *inFlightFences[currentFrame]);


            // ここまで、まだ表示はされない

            // 結果をスワップチェーンに送り返し、画面に表示
            const vk::PresentInfoKHR presentInfoKHR{
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = &*renderFinishedSemaphore[imageIndex],
                .swapchainCount = 1,
                .pSwapchains = &*(swapChain.getSwapChain()),
                .pImageIndices = &imageIndex
            };
            result = device.getQueue().presentKHR(presentInfoKHR);
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR
                // チュートリアルではframebufferResizedの状態を見ていたが、recreateSwapChainしたいだけのようなので変更
                // || framebufferResized
                ) {
                // framebufferResized = false;
                // swapChain.recreateSwapChain();
            }
            else if (result != vk::Result::eSuccess) {
                throw std::runtime_error("failed to present swap chain image!");
            }

            // 次のフレームに進む
            semaphoreIndex = (semaphoreIndex + 1) % presentCompleteSemaphore.size();
            currentFrame = (currentFrame + 1) % maxFrames;
        }

        uint32_t getCurrentFrame() {
            return currentFrame;
        }

        vk::raii::CommandBuffer& getCurrentCommandBuffer() {
            return commandBuffers[currentFrame];
        }

        vk::raii::CommandPool& getCommandPool() {
            return commandPool;
        }

        uint32_t getMaxFrames() {
            return maxFrames;
        }

	private:
        Device& device;
        SwapChain& swapChain;
        vk::raii::CommandPool commandPool = nullptr;
        std::vector<vk::raii::CommandBuffer> commandBuffers;
        std::vector <vk::raii::Semaphore> presentCompleteSemaphore;
        std::vector <vk::raii::Semaphore> renderFinishedSemaphore;
        std::vector <vk::raii::Fence> inFlightFences;
        int maxFrames;
        uint32_t currentFrame = 0;
        uint32_t semaphoreIndex = 0;
	};
}