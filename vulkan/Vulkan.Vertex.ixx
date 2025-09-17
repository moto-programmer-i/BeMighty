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
import :Buffer;


namespace Vulkan {
    // この構造体名で本当に良いのか不明（2次元のpos限定なのか？）
    export struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;


        // ここに書くべきなのか不明、もっと汎用的に書けないか？
        static vk::VertexInputBindingDescription getBindingDescription() {
            return { 0, sizeof(Vertex), vk::VertexInputRate::eVertex };
        }

        // チュートリアルでは要素数を指定していたが、autoとto_arrayがあれば不要
        // static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
        static auto getAttributeDescriptions() {
            return std::to_array({
                // 先頭のindexも自動で割り振れるようにするべき
                vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)),
                vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)),
                vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texCoord))
            });
            
        }
    };


    export class VertexManager {
    public:
        VertexManager(Device& device, Rendering& rendering, std::vector<Vertex> vertices, std::vector<std::uint16_t> indices) :
            // なぜかここで初期化しなければならない
            device(device), rendering(rendering), vertices(vertices), indices(indices)
        {
            
            // インスタンスをバッファにコピーする汎用関数が必要？
            vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
            vk::raii::Buffer stagingBuffer({});
            vk::raii::DeviceMemory stagingBufferMemory({});
            Buffer::createBuffer(device, bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

            void* dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);
            std::memcpy(dataStaging, vertices.data(), bufferSize);
            stagingBufferMemory.unmapMemory();

            Buffer::createBuffer(device, bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer, vertexBufferMemory);

            // CPUからアクセスできるメモリタイプが、グラフィックカード自体が読み取るのに最適なメモリタイプではない可能性がある
            // CPUアクセス可能なメモリ上のバッファ（stagingBufferMemory）と、デバイスのローカルメモリ上が必要。
            // バッファコピーコマンドを使用して、データを移動する
            copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

            
            createIndexbuffer();
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
            Buffer::createBuffer(device, bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

            void* data = stagingBufferMemory.mapMemory(0, bufferSize);
            std::memcpy(data, indices.data(), (size_t)bufferSize);
            stagingBufferMemory.unmapMemory();

            Buffer::createBuffer(device, bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer, indexBufferMemory);

            copyBuffer(stagingBuffer, indexBuffer, bufferSize);
        }
    };
}

