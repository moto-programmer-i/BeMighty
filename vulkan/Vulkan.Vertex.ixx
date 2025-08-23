// https://docs.vulkan.org/tutorial/latest/_attachments/18_vertex_input.cpp
// からコピペ

// offsetofはマクロなので、import std; で含められない
// https://learn.microsoft.com/ja-jp/cpp/cpp/tutorial-import-stl-named-module?view=msvc-170#introduction-to-standard-library-modules
import <cstddef>;

export module Vulkan:Vertex;



import std;

import glm;

// ここはなぜかvulkan_hppをimportしないとエラーになる
import vulkan_hpp;
import :Device; 


namespace Vulkan {
    // この構造体名で本当に良いのか不明（2次元のpos限定なのか？）
    export struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;


        // ここに書くべきなのか不明、もっと汎用的に書けないか？
        static vk::VertexInputBindingDescription getBindingDescription() {
            return { 0, sizeof(Vertex), vk::VertexInputRate::eVertex };
        }

        static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
            return {
                vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, pos)),
                vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color))
            };
        }
    };


    export class VertexManager {
    public:
        VertexManager(Device& device, std::vector<Vertex> vertices) :
            // なぜかここで初期化しなければならない
            device(device), vertices(vertices)
        {
            vk::BufferCreateInfo bufferInfo{ .size = sizeof(vertices[0]) * vertices.size(), .usage = vk::BufferUsageFlagBits::eVertexBuffer, .sharingMode = vk::SharingMode::eExclusive };
            vertexBuffer = vk::raii::Buffer(device.getDevice(), bufferInfo);

            vk::MemoryRequirements memRequirements = vertexBuffer.getMemoryRequirements();
            vk::MemoryAllocateInfo memoryAllocateInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) };
            vertexBufferMemory = vk::raii::DeviceMemory(device.getDevice(), memoryAllocateInfo);

            vertexBuffer.bindMemory(*vertexBufferMemory, 0);

            void* data = vertexBufferMemory.mapMemory(0, bufferInfo.size);
            std::memcpy(data, vertices.data(), bufferInfo.size);
            vertexBufferMemory.unmapMemory();
        }

        // MemoryTypeが列挙体になってないのが多分悪い
        uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
            vk::PhysicalDeviceMemoryProperties memProperties = device.getPhysicalDevice().getMemoryProperties();

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }

            throw std::runtime_error("failed to find suitable memory type!");
        }

        vk::raii::Buffer& getVertexBuffer() {
            return vertexBuffer;
        }
    private:
        Device& device;
        vk::raii::Buffer vertexBuffer = nullptr;
        vk::raii::DeviceMemory vertexBufferMemory = nullptr;
        std::vector<Vertex> vertices;
    };
}

