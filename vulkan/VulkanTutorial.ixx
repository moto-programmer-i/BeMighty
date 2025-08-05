// モジュール宣言の前にincludeしなければエラーになる
#include <algorithm>
// なぜかvulkan_hppに含まれていない
// #include <vulkan/vulkan_core.h>


export module VulkanTutorial;

import std;
import Glfw;


import vulkan_hpp;

import Vulkan;






// Vulkanのチュートリアル
// https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/00_Base_code.html
export class HelloTriangleApplication {
public:
    // チュートリアルではVK_MAKE_VERSIONで作っていたが、不要に見えるので自前で用意。
    // メジャー番号 マイナー番号（2桁）パッチ番号（２桁）を想定
    static constexpr uint32_t APPLICATION_VERSION = 10000;
    static constexpr uint32_t ENGINE_VERSION = 10000;

    HelloTriangleApplication(Glfw::Window* window) {
        this->window = window;
    }
    void run() {
        std::cout << "vulkan3" << std::endl;

        initVulkan();

        // 他はWindowモジュールに移したため、不要
        /*initWindow();
        initVulkan();
        mainLoop();
        cleanup();*/
    }

private:
    // 外部で管理されているウィンドウ。このクラス内では解放しないこと
    Glfw::Window*  window;

    vk::raii::Context  context;
    vk::raii::Instance instance = nullptr;

#ifdef NDEBUG
#else
    // メンバ変数ではautoが使えない

    std::unique_ptr<Vulkan::DebugManager> debugManager;
#endif

    void initVulkan() {
        createInstance();
    }

    void createInstance() {      
        
        constexpr vk::ApplicationInfo appInfo{.pApplicationName = "Hello Triangle",

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

#ifdef NDEBUG
#else
        debugManager = std::make_unique<Vulkan::DebugManager>(instance);
#endif
    }


    // vectorの値渡しがinlineで軽減されてほしい。
    // 一応されなくても頻繁に呼び出す処理ではないので大丈夫なはず
    inline std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        return extensions;
    }
};
