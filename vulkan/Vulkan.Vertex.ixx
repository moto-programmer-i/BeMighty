// https://docs.vulkan.org/tutorial/latest/_attachments/21_index_buffer.cpp
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
import :Rendering;


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
        VertexManager(Device& device, Rendering& rendering, std::vector<Vertex> vertices, std::vector<uint16_t> indices) :
            // なぜかここで初期化しなければならない
            device(device), rendering(rendering), vertices(vertices), indices(indices)
        {
            
            // インスタンスをバッファにコピーする汎用関数が必要？
            vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
            vk::raii::Buffer stagingBuffer({});
            vk::raii::DeviceMemory stagingBufferMemory({});
            createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

            void* dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);
            std::memcpy(dataStaging, vertices.data(), bufferSize);
            stagingBufferMemory.unmapMemory();

            createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer, vertexBufferMemory);

            // CPUからアクセスできるメモリタイプが、グラフィックカード自体が読み取るのに最適なメモリタイプではない可能性がある
            // CPUアクセス可能なメモリ上のバッファ（stagingBufferMemory）と、デバイスのローカルメモリ上が必要。
            // バッファコピーコマンドを使用して、データを移動する
            copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

            
            createIndexbuffer();
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

        void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer& buffer, vk::raii::DeviceMemory& bufferMemory) {
            vk::BufferCreateInfo bufferInfo{ .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive };
            buffer = vk::raii::Buffer(device.getDevice(), bufferInfo);
            vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();
            vk::MemoryAllocateInfo allocInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties) };
            bufferMemory = vk::raii::DeviceMemory(device.getDevice(), allocInfo);
            buffer.bindMemory(bufferMemory, 0);
        }

        void copyBuffer(vk::raii::Buffer& srcBuffer, vk::raii::Buffer& dstBuffer, vk::DeviceSize size) {
            vk::CommandBufferAllocateInfo allocInfo{
                .commandPool = rendering.getCommandPool(),
                .level = vk::CommandBufferLevel::ePrimary,
                .commandBufferCount = 1
            };

            // このmoveが適切か不明
            vk::raii::CommandBuffer commandCopyBuffer = std::move(device.getDevice().allocateCommandBuffers(allocInfo).front());
            
            commandCopyBuffer.begin(vk::CommandBufferBeginInfo{ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
            commandCopyBuffer.copyBuffer(*srcBuffer, *dstBuffer, vk::BufferCopy(0, 0, size));
            commandCopyBuffer.end();
            device.getQueue().submit(vk::SubmitInfo{ .commandBufferCount = 1, .pCommandBuffers = &*commandCopyBuffer }, nullptr);
            device.getQueue().waitIdle();
        }

        vk::raii::Buffer& getVertexBuffer() {
            return vertexBuffer;
        }
        vk::raii::Buffer& getIndexBuffer() {
            return indexBuffer;
        }


        std::vector<uint16_t>& getIndices() {
            return indices;
        }

        vk::IndexType getIndexType() {
            return vk::IndexTypeValue<decltype(indices)::value_type>::value;
        }
    private:
        Device& device;
        Rendering& rendering;
        vk::raii::Buffer vertexBuffer = nullptr;
        vk::raii::DeviceMemory vertexBufferMemory = nullptr;
        vk::raii::Buffer indexBuffer = nullptr;
        vk::raii::DeviceMemory indexBufferMemory = nullptr;
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        void createIndexbuffer() {
            vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

            vk::raii::Buffer stagingBuffer({});
            vk::raii::DeviceMemory stagingBufferMemory({});
            createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

            void* data = stagingBufferMemory.mapMemory(0, bufferSize);
            std::memcpy(data, indices.data(), (size_t)bufferSize);
            stagingBufferMemory.unmapMemory();

            createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer, indexBufferMemory);

            copyBuffer(stagingBuffer, indexBuffer, bufferSize);
        }
    };
}

