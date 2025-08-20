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

        /// <summary>
        /// 1フレームを描画
        /// </summary>
        /// <param name="recordCommandBuffer">imageIndexを使って描画するコマンド</param>
        void drawFrame(std::function<void(uint32_t)> recordCommandBuffer) {
            // 前のコマンドが終了するまで待つ
            device.getQueue().waitIdle();

            

            auto [result, imageIndex] = swapChain.getSwapChain().acquireNextImage(
                // 仕様の資料では、3番目の引数がtimeoutだが、実際は1番目
                // timeout specifies how long the function waits, in nanoseconds, if no image is available.
                // https://registry.khronos.org/vulkan/specs/latest/man/html/vkAcquireNextImageKHR.html
                UINT64_MAX,

                // セマフォ
                *presentCompleteSemaphore,

                //  VkFence
                // セマフォのように、同期のための仕組みらしい
                // GPUが何かを完了したことをホストが知る必要がある場合にフェンスを使用
                // https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/03_Drawing/02_Rendering_and_presentation.html#_fences
                nullptr);

            // 指定したものを描画
            recordCommandBuffer(imageIndex);


            device.getDevice().resetFences(*drawFence);

            // キューの送信と同期
            // パイプラインの実行段階を指定している？
            // https://docs.vulkan.org/spec/latest/_images/pipelinemesh.svg
            vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
            const vk::SubmitInfo submitInfo{
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = &*presentCompleteSemaphore,
                .pWaitDstStageMask = &waitDestinationStageMask,

                // コマンドをバッチ実行する数
                .commandBufferCount = 1,
                .pCommandBuffers = &*(command.getCommandBuffer()),

                // 実行後のセマフォの紐づけ
                .pSignalSemaphores = &*renderFinishedSemaphore
            };
            device.getQueue().submit(submitInfo, *drawFence);


            // 実行終了まで待機
            while (vk::Result::eTimeout == device.getDevice().waitForFences(*drawFence, vk::True, UINT64_MAX))
                ;

            // ここまで、まだ表示はされない

            // 結果をスワップチェーンに送り返し、画面に表示
            const vk::PresentInfoKHR presentInfoKHR{
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = &*renderFinishedSemaphore,
                .swapchainCount = 1,
                .pSwapchains = &*(swapChain.getSwapChain()),
                .pImageIndices = &imageIndex
            };
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