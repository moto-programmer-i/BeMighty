// https://docs.vulkan.org/tutorial/latest/_attachments/20_staging_buffer.cpp
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
        VertexManager(Device& device, Rendering& rendering, std::vector<Vertex> vertices) :
            // なぜかここで初期化しなければならない
            device(device), rendering(rendering), vertices(vertices)
        {
            vk::BufferCreateInfo stagingInfo{ .size = sizeof(vertices[0]) * vertices.size(), .usage = vk::BufferUsageFlagBits::eTransferSrc, .sharingMode = vk::SharingMode::eExclusive };
            vk::raii::Buffer stagingBuffer(device.getDevice(), stagingInfo);
            vk::MemoryRequirements memRequirementsStaging = stagingBuffer.getMemoryRequirements();

            // 実際のアプリケーションでは、vkAllocateMemoryすべてのバッファを個別に呼び出す必要はないらしい
            // じゃあどうすれば良いのか不明
            // https://docs.vulkan.org/tutorial/latest/04_Vertex_buffers/02_Staging_buffer.html
            vk::MemoryAllocateInfo memoryAllocateInfoStaging{ .allocationSize = memRequirementsStaging.size, .memoryTypeIndex = findMemoryType(memRequirementsStaging.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) };
            vk::raii::DeviceMemory stagingBufferMemory(device.getDevice(), memoryAllocateInfoStaging);
            stagingBuffer.bindMemory(stagingBufferMemory, 0);
            void* dataStaging = stagingBufferMemory.mapMemory(0, stagingInfo.size);
            std::memcpy(dataStaging, vertices.data(), stagingInfo.size);
            stagingBufferMemory.unmapMemory();

            vk::BufferCreateInfo bufferInfo{ .size = sizeof(vertices[0]) * vertices.size(),  .usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, .sharingMode = vk::SharingMode::eExclusive };
            vertexBuffer = vk::raii::Buffer(device.getDevice(), bufferInfo);

            vk::MemoryRequirements memRequirements = vertexBuffer.getMemoryRequirements();
            vk::MemoryAllocateInfo memoryAllocateInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal) };
            vertexBufferMemory = vk::raii::DeviceMemory(device.getDevice(), memoryAllocateInfo);

            vertexBuffer.bindMemory(*vertexBufferMemory, 0);

            // CPUからアクセスできるメモリタイプが、グラフィックカード自体が読み取るのに最適なメモリタイプではない可能性がある
            // CPUアクセス可能なメモリ上のバッファ（stagingBufferMemory）と、デバイスのローカルメモリ上が必要。
            // バッファコピーコマンドを使用して、データを移動する
            copyBuffer(stagingBuffer, vertexBuffer, stagingInfo.size);
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
    private:
        Device& device;
        Rendering& rendering;
        vk::raii::Buffer vertexBuffer = nullptr;
        vk::raii::DeviceMemory vertexBufferMemory = nullptr;
        std::vector<Vertex> vertices;
    };
}

