export module Vulkan:SwapChainSettings;

import std;
using namespace std;

import vulkan_hpp;

namespace Vulkan {
    // ���݂̓`���[�g���A���̒l�������l�Ƃ��Ă���
    // https://docs.vulkan.org/tutorial/latest/_attachments/07_image_views.cpp
    export constexpr vk::ImageAspectFlags DEFAULT_ASPECT_MASK = vk::ImageAspectFlagBits::eColor;
    export constexpr uint32_t DEFAULT_BASE_MIP_LEVEL = 0;
    export constexpr uint32_t DEFAULT_LEVEL_COUNT = 1;
    export constexpr uint32_t DEFAULT_BASE_ARRAY_LAYER = 0;
    export constexpr uint32_t DEFAULT_LAYER_COUNT = 1;

    // https://registry.khronos.org/vulkan/specs/latest/man/html/VkImageSubresourceRange.html
	export struct SwapChainSettings {
        vk::ImageAspectFlags aspectMask = vk::ImageAspectFlagBits::eColor;

        // baseMipLevel is the first mipmap level accessible to the view.
        // mipmap�F�������̂͒Ⴂ�𑜓x�ŕ\�������A��
        // https://docs.unity3d.com/ja/2021.3/Manual/texture-mipmaps-introduction.html
        uint32_t baseMipLevel = DEFAULT_BASE_MIP_LEVEL;

        // levelCount is the number of mipmap levels(starting from baseMipLevel) accessible to the view.
        // �����܂Ŏw��ł���̂��ȂǁA�ڍׂ͕s��
        uint32_t levelCount = DEFAULT_LEVEL_COUNT;

        // �s���BbaseArrayLayer is the first array layer accessible to the view.
        uint32_t baseArrayLayer = DEFAULT_BASE_ARRAY_LAYER;

        // layerCount is the number of array layers(starting from baseArrayLayer) accessible to the view.
        uint32_t layerCount = DEFAULT_LAYER_COUNT;
	};
}