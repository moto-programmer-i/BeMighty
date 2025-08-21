﻿// モジュール宣言の前にincludeしなければエラーになる
#include <algorithm>

export module Vulkan:Information;

import std;

import Glfw;

import vulkan_hpp;



// import :Validation;


// https://docs.vulkan.org/tutorial/latest/_attachments/09_shader_modules.cpp
// からコピペ

namespace Vulkan {
    // 本当は:Validationモジュールに含めたいが、vk::raii::Instance作成時に必要なので現状しょうがない
    // レイヤーをなぜかこちらで文字列指定しなければいけない
    export const std::vector VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation"
    };

    export void addDebugExtensions(std::vector<const char*>& extensions) {
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
    }


	// vk::raii::Instanceをもつクラス。
	// Instanceという名前のクラスはどうかと思うので変更。
	export class Information {
	public:
        // チュートリアルではVK_MAKE_VERSIONで作っていたが、エラーになる上に不要に見えるので自前で用意。
        // メジャー番号 マイナー番号（2桁）パッチ番号（２桁）を想定
        static constexpr uint32_t APPLICATION_VERSION = 10000;
        static constexpr uint32_t ENGINE_VERSION = 10000;

        

		Information(Glfw::Window& window)
            // 参照はここで初期化しなければならない
            // https://blog.hamayanhamayan.com/entry/2017/11/27/200917
            : window(window) {
            constexpr vk::ApplicationInfo appInfo{ .pApplicationName = "Hello Triangle",

                // import vulkan_hpp;だとなぜかVK_MAKE_VERSIONが未定義。
                // しかも、非推奨の機能のようなので一旦無視して、独自に適当に番号をつける
                // .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                // 推奨はこれだが、なぜかモジュールには入っていない
                // .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                .applicationVersion = APPLICATION_VERSION,


                .pEngineName = "No Engine",

                // .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                // 推奨はこれだが、なぜかモジュールには入っていない
                // .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),

                .engineVersion = ENGINE_VERSION,

                .apiVersion = vk::ApiVersion14 };


            std::vector<char const*> requiredLayers;
            auto requiredExtensions = getRequiredExtensions();

            // できればここのifdefもなくしたいが、現状しょうがない
#ifdef NDEBUG
#else
        // デバッグ用の情報がcreateInstance時に必要
            requiredLayers.append_range(Vulkan::VALIDATION_LAYERS);
            Vulkan::addDebugExtensions(requiredExtensions);
#endif



            // Check if the required layers are supported by the Vulkan implementation.
            auto layerProperties = context.enumerateInstanceLayerProperties();
            for (auto const& requiredLayer : requiredLayers)
            {
                if (std::ranges::none_of(layerProperties,
                    [requiredLayer](auto const& layerProperty)
                    { return strcmp(layerProperty.layerName, requiredLayer) == 0; }))
                {
                    throw std::runtime_error("Required layer not supported: " + std::string(requiredLayer));
                }
            }



            // 必須extensionsがVulkanの実装でサポートされているかチェック
            auto extensionProperties = context.enumerateInstanceExtensionProperties();
            for (auto const& requiredExtension : requiredExtensions)
            {
                if (std::ranges::none_of(extensionProperties,
                    [requiredExtension](auto const& extensionProperty)
                    { return strcmp(extensionProperty.extensionName, requiredExtension) == 0; }))
                {
                    throw std::runtime_error("Required extension not supported: " + std::string(requiredExtension));
                }
            }

            vk::InstanceCreateInfo createInfo{
                .pApplicationInfo = &appInfo,
                .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
                .ppEnabledLayerNames = requiredLayers.data(),
                .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
                .ppEnabledExtensionNames = requiredExtensions.data() };
            instance = vk::raii::Instance(context, createInfo);

            createSurface();
		}

        vk::raii::Instance& getInstance() {
            return instance;
        }

        vk::raii::SurfaceKHR& getSurface() {
            return surface;
        }
	private:
        // 外部で管理されているウィンドウ。このクラス内では解放しないこと
        Glfw::Window& window;

		vk::raii::Context  context;
		vk::raii::Instance instance = nullptr;
		vk::raii::SurfaceKHR surface = nullptr;

        // ムーブが優先されるのでvectorは値渡しにならないはず
        // https://zenn.dev/dec9ue/books/8c59757478a547/viewer/83003e
        std::vector<const char*> getRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
            return extensions;
        }

        void createSurface() {
            VkSurfaceKHR       _surface;
            if (glfwCreateWindowSurface(*instance, &window.getWindow(), nullptr, &_surface) != 0) {
                throw std::runtime_error("failed to create window surface!");
            }
            surface = vk::raii::SurfaceKHR(instance, _surface);
        }
	};
}