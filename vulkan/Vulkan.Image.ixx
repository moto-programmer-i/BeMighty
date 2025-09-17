// https://docs.vulkan.org/tutorial/latest/_attachments/27_depth_buffering.cpp
// からコピペ

export module Vulkan:Image;

import std;

import vulkan_hpp;

import :Settings;
import :Device;
import :Buffer;


namespace Vulkan {
    export class Image {
    public:
        // 本当はImageと DeviceMemoryを返す関数にするべきかも
        static void createImage(Device& device, std::uint32_t width, std::uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Image& image, vk::raii::DeviceMemory& imageMemory) {
            vk::ImageCreateInfo imageInfo{ .imageType = vk::ImageType::e2D, .format = format,
                                          .extent = {width, height, 1}, .mipLevels = 1, .arrayLayers = 1,
                                          .samples = vk::SampleCountFlagBits::e1, .tiling = tiling,
                                          .usage = usage, .sharingMode = vk::SharingMode::eExclusive };

            image = vk::raii::Image(device.getDevice(), imageInfo);

            vk::MemoryRequirements memRequirements = image.getMemoryRequirements();
            vk::MemoryAllocateInfo allocInfo{ .allocationSize = memRequirements.size,
                                             .memoryTypeIndex = Buffer::findMemoryType(device, memRequirements.memoryTypeBits, properties) };
            imageMemory = vk::raii::DeviceMemory(device.getDevice(), allocInfo);
            image.bindMemory(imageMemory, 0);
        }

        static vk::raii::ImageView createImageView(Device& device, vk::raii::Image& image, vk::Format format, vk::ImageAspectFlags aspectFlags) {
            vk::ImageViewCreateInfo viewInfo{
                .image = image,
                .viewType = vk::ImageViewType::e2D,
                .format = format,
                .subresourceRange = { aspectFlags, 0, 1, 0, 1 }
            };
            return vk::raii::ImageView(device.getDevice(), viewInfo);
        }
    };
}