#include <algorithm>


export module Vulkan:SwapChain;

import std;

import Glfw;

// Deviceでimportされているので不要？もしくはvulkan_hppモジュールの問題かも
// import vulkan_hpp;
import :Device;
import :SwapChainSettings;

// https://docs.vulkan.org/tutorial/latest/_attachments/06_swap_chain_creation.cpp
// からコピペ

namespace Vulkan {
	export class SwapChain {
	public:
        SwapChain(Vulkan::Device& device, vk::raii::SurfaceKHR& surface, Glfw::Window& window,
            Vulkan::SwapChainSettings settings = {}
            )
            // 参照はここで初期化しなければならない
            // https://blog.hamayanhamayan.com/entry/2017/11/27/200917
            : window(window)
        {
            auto surfaceCapabilities = device.getPhysicalDevice().getSurfaceCapabilitiesKHR(surface);
            swapChainImageFormat = chooseSwapSurfaceFormat(device.getPhysicalDevice().getSurfaceFormatsKHR(surface));
            swapChainExtent = chooseSwapExtent(surfaceCapabilities);
            auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
            minImageCount = (surfaceCapabilities.maxImageCount > 0 && minImageCount > surfaceCapabilities.maxImageCount) ? surfaceCapabilities.maxImageCount : minImageCount;
            vk::SwapchainCreateInfoKHR swapChainCreateInfo{
                .surface = surface, .minImageCount = minImageCount,
                .imageFormat = swapChainImageFormat, .imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear,
                .imageExtent = swapChainExtent, .imageArrayLayers = 1,
                .imageUsage = vk::ImageUsageFlagBits::eColorAttachment, .imageSharingMode = vk::SharingMode::eExclusive,
                .preTransform = surfaceCapabilities.currentTransform, .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
                .presentMode = chooseSwapPresentMode(device.getPhysicalDevice().getSurfacePresentModesKHR(surface)),
                .clipped = true };

            swapChain = vk::raii::SwapchainKHR(device.getDevice(), swapChainCreateInfo);
            swapChainImages = swapChain.getImages();

            createImageViews(device, settings);
        }

        static vk::Format chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
            const auto formatIt = std::ranges::find_if(availableFormats,
                [](const auto& format) {
                    return format.format == vk::Format::eB8G8R8A8Srgb &&
                        format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
                });
            return formatIt != availableFormats.end() ? formatIt->format : availableFormats[0].format;
        }

        static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
            return std::ranges::any_of(availablePresentModes,
                [](const vk::PresentModeKHR value) { return vk::PresentModeKHR::eMailbox == value; }) ? vk::PresentModeKHR::eMailbox : vk::PresentModeKHR::eFifo;
        }

        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            }
            auto framebufferSize = window.getFramebufferSize();

            return {
                std::clamp<uint32_t>(framebufferSize.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                std::clamp<uint32_t>(framebufferSize.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
            };
        }


        std::vector<vk::Image>& getSwapChainImages() {
            return swapChainImages;
        }

        vk::Format& getSwapChainImageFormat() {
            return swapChainImageFormat;
        }

        std::vector<vk::raii::ImageView>& getSwapChainImageViews() {
            return swapChainImageViews;
        }

        vk::Extent2D& getSwapChainExtent() {
            return swapChainExtent;
        }

	private:
        Glfw::Window& window;
		vk::raii::SwapchainKHR swapChain = nullptr;
		std::vector<vk::Image> swapChainImages;
		vk::Format swapChainImageFormat = vk::Format::eUndefined;
		vk::Extent2D swapChainExtent;
		std::vector<vk::raii::ImageView> swapChainImageViews;

        void createImageViews(Vulkan::Device& device, Vulkan::SwapChainSettings& settings) {
            swapChainImageViews.clear();

            vk::ImageViewCreateInfo imageViewCreateInfo{
                .viewType = vk::ImageViewType::e2D,
                .format = swapChainImageFormat,
                // https://registry.khronos.org/vulkan/specs/latest/man/html/VkImageSubresourceRange.html
                .subresourceRange = { 
                    .aspectMask = settings.aspectMask,
                    .baseMipLevel = settings.baseMipLevel,
                    .levelCount = settings.levelCount,
                    .baseArrayLayer = settings.baseArrayLayer,
                    .layerCount = settings.layerCount 
                }
            };

            // &imageにするよう警告がでているが、どちらが正しいか不明
            for (auto image : swapChainImages)
            {
                imageViewCreateInfo.image = image;
                swapChainImageViews.emplace_back(device.getDevice(), imageViewCreateInfo);
            }
        }

	};
}

