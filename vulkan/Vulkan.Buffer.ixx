// https://docs.vulkan.org/tutorial/latest/_attachments/23_descriptor_sets.cpp
// からコピペ




export module Vulkan:Buffer;



import std;


#define GLM_FORCE_RADIANS
import glm;
// IntelliSense上ではこっちのほうが便利だが、ビルドが通らなくなる
// #include <glm/glm.hpp>



// ここはなぜかvulkan_hppをimportしないとIntelisenseエラーになる
import vulkan_hpp;
import :Device;


namespace Vulkan {
    export struct UniformBufferObject {
        // IntelliSenseではエラーがでるが、ビルドでは問題ない
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    
    
    export class Buffer {
    public:
        static void createBuffer(Device& device, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory) {
            vk::BufferCreateInfo bufferInfo{ .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive };
            buffer = vk::raii::Buffer(device.getDevice(), bufferInfo);
            vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();
            vk::MemoryAllocateInfo allocInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(device, memRequirements.memoryTypeBits, properties) };
            bufferMemory = vk::raii::DeviceMemory(device.getDevice(), allocInfo);
            buffer.bindMemory(bufferMemory, 0);
        }

        // MemoryTypeが列挙体になってないのが多分悪い
        static uint32_t findMemoryType(Device& device, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
            vk::PhysicalDeviceMemoryProperties memProperties = device.getPhysicalDevice().getMemoryProperties();

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }

            throw std::runtime_error("failed to find suitable memory type!");
        }
    private:
        
    };

    
    export class UniformBufferManager {
    public:
        UniformBufferManager(Device& device, const uint32_t size):
            // なぜかここで初期化しなければならない
            size(size)
        {
            uniformBuffers.clear();
            uniformBuffersMemory.clear();
            uniformBuffersMapped.clear();

            vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

            for (size_t i = 0; i < size; i++) {
                // nullptrでインスタンスを作るのは変だが、vk::raii::Bufferがそうなっているのでしょうがない
                auto& buffer = uniformBuffers.emplace_back(nullptr);
                auto& bufferMem = uniformBuffersMemory.emplace_back(nullptr);
                Buffer::createBuffer(device, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, buffer, bufferMem);
                uniformBuffersMapped.emplace_back(uniformBuffersMemory[i].mapMemory(0, bufferSize));


                // チュートリアルではemplace_backをうまく使えていない
                //vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
                //vk::raii::Buffer buffer({});
                //vk::raii::DeviceMemory bufferMem({});
                //Buffer::createBuffer(device, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, buffer, bufferMem);
                //uniformBuffers.emplace_back(std::move(buffer));
                //uniformBuffersMemory.emplace_back(std::move(bufferMem));
                //uniformBuffersMapped.emplace_back(uniformBuffersMemory[i].mapMemory(0, bufferSize));
            }
        }

        uint32_t getSize() {
            return size;
        }

        vk::raii::Buffer& getUniformBuffer(size_t index) {
            return uniformBuffers[index];
        }
        
        void* getUniformBufferMapped(uint32_t currentImage) {
            return uniformBuffersMapped[currentImage];
        }

    private:
        uint32_t size;
        std::vector<vk::raii::Buffer> uniformBuffers;
        std::vector<vk::raii::DeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;
    };
}