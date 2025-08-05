// ���W���[���錾�̑O��include���Ȃ���΃G���[�ɂȂ�
#include <algorithm>
// �Ȃ���vulkan_hpp�Ɋ܂܂�Ă��Ȃ�
// #include <vulkan/vulkan_core.h>


export module VulkanTutorial;

import std;
import Glfw;


import vulkan_hpp;

import Vulkan;






// Vulkan�̃`���[�g���A��
// https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/00_Base_code.html
export class HelloTriangleApplication {
public:
    // �`���[�g���A���ł�VK_MAKE_VERSION�ō���Ă������A�s�v�Ɍ�����̂Ŏ��O�ŗp�ӁB
    // ���W���[�ԍ� �}�C�i�[�ԍ��i2���j�p�b�`�ԍ��i�Q���j��z��
    static constexpr uint32_t APPLICATION_VERSION = 10000;
    static constexpr uint32_t ENGINE_VERSION = 10000;

    HelloTriangleApplication(Glfw::Window* window) {
        this->window = window;
    }
    void run() {
        std::cout << "vulkan3" << std::endl;

        initVulkan();

        // ����Window���W���[���Ɉڂ������߁A�s�v
        /*initWindow();
        initVulkan();
        mainLoop();
        cleanup();*/
    }

private:
    // �O���ŊǗ�����Ă���E�B���h�E�B���̃N���X���ł͉�����Ȃ�����
    Glfw::Window*  window;

    vk::raii::Context  context;
    vk::raii::Instance instance = nullptr;

#ifdef NDEBUG
#else
    // �����o�ϐ��ł�auto���g���Ȃ�

    std::unique_ptr<Vulkan::DebugManager> debugManager;
#endif

    void initVulkan() {
        createInstance();
    }

    void createInstance() {      
        
        constexpr vk::ApplicationInfo appInfo{.pApplicationName = "Hello Triangle",

            // import vulkan_hpp;���ƂȂ���VK_MAKE_VERSION������`�B
            // �������A�񐄏��̋@�\�̂悤�Ȃ̂ň�U�������āA�Ǝ��ɓK���ɔԍ�������
            // .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            // �����͂��ꂾ���A�Ȃ������W���[���ɂ͓����Ă��Ȃ�
            // .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
            .applicationVersion = APPLICATION_VERSION,
            
            
            .pEngineName = "No Engine",

            // .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            // �����͂��ꂾ���A�Ȃ������W���[���ɂ͓����Ă��Ȃ�
            // .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
            
            .engineVersion = ENGINE_VERSION,
            
            .apiVersion = vk::ApiVersion14 };

        
        std::vector<char const*> requiredLayers;
        auto requiredExtensions = getRequiredExtensions();

// �ł���΂�����ifdef���Ȃ����������A���󂵂傤���Ȃ�
#ifdef NDEBUG
#else
        // �f�o�b�O�p�̏��createInstance���ɕK�v
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

        

        // �K�{extensions��Vulkan�̎����ŃT�|�[�g����Ă��邩�`�F�b�N
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


    // vector�̒l�n����inline�Ōy������Ăق����B
    // �ꉞ����Ȃ��Ă��p�ɂɌĂяo�������ł͂Ȃ��̂ő��v�Ȃ͂�
    inline std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        return extensions;
    }
};
