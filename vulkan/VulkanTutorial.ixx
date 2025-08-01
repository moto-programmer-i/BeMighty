// モジュール宣言の前にincludeしなければエラーになる
#include <algorithm>

export module VulkanTutorial;

import std;
import Glfw;
import vulkan_hpp;

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
        std::cout << "vulkan" << std::endl;

        initVulkan();

        // 他はWindowモジュールに移したため、不要
        /*initWindow();
        initVulkan();
        mainLoop();
        cleanup();*/
    }

private:
    // 外部で管理されているウィンドウ。このクラス内では解放しないこと
    Glfw::Window *  window;

    vk::raii::Context  context;
    vk::raii::Instance instance = nullptr;

    void initVulkan() {
        createInstance();
    }

    void createInstance() {      
        
        constexpr vk::ApplicationInfo appInfo{.pApplicationName = "Hello Triangle",

            // import vulkan_hpp;だとなぜかVK_MAKE_VERSIONが未定義。
            // しかも、非推奨の機能のようなので一旦無視して、独自に適当に番号をつける
            // .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .applicationVersion = APPLICATION_VERSION,
            
            .pEngineName = "No Engine",
            // .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .engineVersion = ENGINE_VERSION,
            
            .apiVersion = vk::ApiVersion14 };
        
        

        // Get the required instance extensions from GLFW.
        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


        // Check if the required GLFW extensions are supported by the Vulkan implementation.
        auto extensionProperties = context.enumerateInstanceExtensionProperties();
        for (uint32_t i = 0; i < glfwExtensionCount; ++i)
        {
            if (std::ranges::none_of(extensionProperties,
                [glfwExtension = glfwExtensions[i]](auto const& extensionProperty)
                { return strcmp(extensionProperty.extensionName, glfwExtension) == 0; }))
            
            {
                throw std::runtime_error("Required GLFW extension not supported: " + std::string(glfwExtensions[i]));
            }
        }

        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = glfwExtensionCount,
            .ppEnabledExtensionNames = glfwExtensions };
        instance = vk::raii::Instance(context, createInfo);
    }
};
