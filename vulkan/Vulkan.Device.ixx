
// #include <memory>
#include <algorithm>


// import vulkan_hpp;してもVK_API_VERSION_1_3がなぜか未定義
#include <vulkan/vulkan_core.h>
// #include <vulkan/vk_platform.h>

export module Vulkan:Device;

import std;

import vulkan_hpp;



namespace Vulkan {
	// 確定で使う機能は定義したい
	// チューリアルのコピペだが、この機能群がデフォルトとして適切かは不明
	// https://docs.vulkan.org/tutorial/latest/_attachments/06_swap_chain_creation.cpp
	export const std::vector<const char*> DEFAULT_DEVICE_EXTENSION = {
		vk::KHRSwapchainExtensionName,
		vk::KHRSpirv14ExtensionName,
		vk::KHRSynchronization2ExtensionName,
		vk::KHRCreateRenderpass2ExtensionName
	};

    // チュートリアルからモジュール化のため整理
    // https://docs.vulkan.org/tutorial/latest/_attachments/06_swap_chain_creation.cpp

	export class Device {
	public:
		Device(vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface, std::vector<const char*> requiredDeviceExtension = DEFAULT_DEVICE_EXTENSION) {
			pickPhysicalDevice(instance, requiredDeviceExtension);
			createLogicalDevice(instance, surface, requiredDeviceExtension);
		}

        vk::raii::PhysicalDevice& getPhysicalDevice() {
            return physicalDevice;
        }

        vk::raii::Device& getDevice() {
            return device;
        }

        vk::raii::Queue& getQueue() {
            return queue;
        }

        uint32_t& getQueueIndex() {
            return queueIndex;
        }
        
	private:
		vk::raii::PhysicalDevice         physicalDevice = nullptr;
		vk::raii::Device                 device = nullptr;
        vk::raii::Queue                  queue = nullptr;
        uint32_t                         queueIndex = ~0;

        void pickPhysicalDevice(vk::raii::Instance& instance, std::vector<const char*>& requiredDeviceExtension) {
            std::vector<vk::raii::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
            const auto                            devIter = std::ranges::find_if(
                devices,
                [&](auto const& device)
                {
                    // Check if the device supports the Vulkan 1.3 API version
                    bool supportsVulkan1_3 = device.getProperties().apiVersion >= VK_API_VERSION_1_3;

                    // Check if any of the queue families support graphics operations
                    auto queueFamilies = device.getQueueFamilyProperties();
                    bool supportsGraphics =
                        std::ranges::any_of(queueFamilies, [](auto const& qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });

                    // Check if all required device extensions are available
                    auto availableDeviceExtensions = device.enumerateDeviceExtensionProperties();
                    bool supportsAllRequiredExtensions =
                        std::ranges::all_of(requiredDeviceExtension,
                            [&availableDeviceExtensions](auto const& requiredDeviceExtension)
                            {
                                return std::ranges::any_of(availableDeviceExtensions,
                                    [requiredDeviceExtension](auto const& availableDeviceExtension)
                                    { return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0; });
                            });

                    auto features = device.template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
                    bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                        features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

                    return supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
                });
            if (devIter != devices.end())
            {
                physicalDevice = *devIter;
            }
            else
            {
                throw std::runtime_error("failed to find a suitable GPU!");
            }
        }

        void createLogicalDevice(vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface, std::vector<const char*>& requiredDeviceExtension) {
            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

            // get the first index into queueFamilyProperties which supports both graphics and present
            // メンバ変数に移動された
            // https://docs.vulkan.org/tutorial/latest/_attachments/14_command_buffers.cpp
            // uint32_t queueIndex = ~0;
            for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++)
            {
                if ((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
                    physicalDevice.getSurfaceSupportKHR(qfpIndex, *surface))
                {
                    // found a queue family that supports both graphics and present
                    queueIndex = qfpIndex;
                    break;
                }
            }
            if (queueIndex == ~0)
            {
                throw std::runtime_error("Could not find a queue for graphics and present -> terminating");
            }

            // query for Vulkan 1.3 features
            vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain = {
                {},                               // vk::PhysicalDeviceFeatures2
                {.dynamicRendering = true },      // vk::PhysicalDeviceVulkan13Features
                {.extendedDynamicState = true }   // vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
            };

            // create a Device
            float                     queuePriority = 0.0f;
            vk::DeviceQueueCreateInfo deviceQueueCreateInfo{ .queueFamilyIndex = queueIndex, .queueCount = 1, .pQueuePriorities = &queuePriority };
            vk::DeviceCreateInfo      deviceCreateInfo{ .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
                                                        .queueCreateInfoCount = 1,
                                                        .pQueueCreateInfos = &deviceQueueCreateInfo,
                                                        .enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
                                                        .ppEnabledExtensionNames = requiredDeviceExtension.data() };

            device = vk::raii::Device(physicalDevice, deviceCreateInfo);
            queue = vk::raii::Queue(device, queueIndex, 0);
        }
	};
}