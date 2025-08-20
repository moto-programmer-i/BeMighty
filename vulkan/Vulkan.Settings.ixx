export module Vulkan:Settings;

import std;

import vulkan_hpp;


namespace Vulkan {
	// 面倒なので全体的に設定用のクラス。でかくなりすぎたら変更するかも
	export class Settings {
	public:

		// 現在はチュートリアルの値を初期値としている
		// https://docs.vulkan.org/tutorial/latest/_attachments/07_image_views.cpp
		static constexpr vk::ImageAspectFlags DEFAULT_ASPECT_MASK = vk::ImageAspectFlagBits::eColor;
		static constexpr std::uint32_t DEFAULT_BASE_MIP_LEVEL = 0;
		static constexpr std::uint32_t DEFAULT_LEVEL_COUNT = 1;
		static constexpr std::uint32_t DEFAULT_BASE_ARRAY_LAYER = 0;
		static constexpr std::uint32_t DEFAULT_LAYER_COUNT = 1;


		static vk::ImageSubresourceRange defaultImageSubresourceRange() {
			return {
				.aspectMask = vk::ImageAspectFlagBits::eColor,

				// baseMipLevel is the first mipmap level accessible to the view.
				// mipmap：遠いものは低い解像度で表示されるアレ
				// https://docs.unity3d.com/ja/2021.3/Manual/texture-mipmaps-introduction.html
				.baseMipLevel = DEFAULT_BASE_MIP_LEVEL,

				// levelCount is the number of mipmap levels(starting from baseMipLevel) accessible to the view.
				// いくつまで指定できるのかなど、詳細は不明
				.levelCount = DEFAULT_LEVEL_COUNT,

				// 不明。baseArrayLayer is the first array layer accessible to the view.
				.baseArrayLayer = DEFAULT_BASE_ARRAY_LAYER,

				// layerCount is the number of array layers(starting from baseArrayLayer) accessible to the view.
				.layerCount = DEFAULT_LAYER_COUNT
			};
		}
	private:
	};
}