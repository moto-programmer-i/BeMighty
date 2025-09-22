// https://docs.vulkan.org/tutorial/latest/_attachments/23_descriptor_sets.cpp
// からコピペ

export module Vulkan:Descriptor;

import std;

import vulkan_hpp;

import :Device;
import :GraphicsPipeline;
import :Buffer;
import :Texture;

namespace Vulkan {
    export class Descriptor {
    public:
        // https://docs.vulkan.org/tutorial/latest/_attachments/22_descriptor_layout.cpp
        Descriptor(Device& device, GraphicsPipeline& graphicsPipeline, UniformBufferManager& uniformBufferManager, Texture& texture) {
            // GraphicsPipelineでdescriptorSetLayoutが必要なので移動
            //// createDescriptorSetLayout
            //std::array bindings = {
            //    // 最初の引数はbindingのindex、本当はDescriptorSetLayoutを作るstatic関数が必要
            //    vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr),
            //    vk::DescriptorSetLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment, nullptr)
            //};
            //vk::DescriptorSetLayoutCreateInfo layoutInfo{ .bindingCount = static_cast<uint32_t>(bindings.size()), .pBindings = bindings.data() };
            //descriptorSetLayout = vk::raii::DescriptorSetLayout(device.getDevice(), layoutInfo);

            // createDescriptorPool
            std::array poolSize{
                vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, uniformBufferManager.getSize()),
                vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler,uniformBufferManager.getSize())
            };
            vk::DescriptorPoolCreateInfo poolInfo{
                .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
                .maxSets = uniformBufferManager.getSize(),
                .poolSizeCount = static_cast<uint32_t>(poolSize.size()),
                .pPoolSizes = poolSize.data()
            };
            descriptorPool = vk::raii::DescriptorPool(device.getDevice(), poolInfo);

            // createDescriptorSets
            std::vector<vk::DescriptorSetLayout> layouts(
                uniformBufferManager.getSize(),
                graphicsPipeline.getDescriptorSetLayout()
            );
            vk::DescriptorSetAllocateInfo allocInfo{
                .descriptorPool = descriptorPool,
                .descriptorSetCount = static_cast<uint32_t>(layouts.size()),
                .pSetLayouts = layouts.data()
            };

            descriptorSets.clear();
            descriptorSets = device.getDevice().allocateDescriptorSets(allocInfo);

            for (size_t i = 0; i < uniformBufferManager.getSize(); ++i) {
                vk::DescriptorBufferInfo bufferInfo{
                    .buffer = uniformBufferManager.getUniformBuffer(i),
                    .offset = 0,
                    .range = sizeof(UniformBufferObject)
                };
                vk::DescriptorImageInfo imageInfo{
                    .sampler = texture.getTextureSampler(),
                    .imageView = texture.getTextureImageView(),
                    .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
                };
                std::array descriptorWrites{
                    vk::WriteDescriptorSet{
                        .dstSet = descriptorSets[i],
                        .dstBinding = 0,
                        .dstArrayElement = 0,
                        .descriptorCount = 1,
                        .descriptorType = vk::DescriptorType::eUniformBuffer,
                        .pBufferInfo = &bufferInfo
                    },
                    vk::WriteDescriptorSet{
                        .dstSet = descriptorSets[i],
                        .dstBinding = 1,
                        .dstArrayElement = 0,
                        .descriptorCount = 1,
                        .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                        .pImageInfo = &imageInfo
                    }
                };
                device.getDevice().updateDescriptorSets(descriptorWrites, {});
            }
        }

        // GraphicsPipelineでdescriptorSetLayoutが必要なので移動
        //vk::raii::DescriptorSetLayout& getDescriptorSetLayout() {
        //    return descriptorSetLayout;
        //}

        vk::raii::DescriptorSet& getDescriptorSet(size_t i) {
            return descriptorSets[i];
        }
    private:
        // GraphicsPipelineでdescriptorSetLayoutが必要なので移動
        // vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;

        // Vulkanの仕様上必須なだけで、本来は不要
        vk::raii::DescriptorPool descriptorPool = nullptr;


        std::vector<vk::raii::DescriptorSet> descriptorSets;
    };
}