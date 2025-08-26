// https://docs.vulkan.org/tutorial/latest/_attachments/23_descriptor_sets.cpp
// からコピペ

export module Vulkan:Descriptor;

import std;

import vulkan_hpp;

import :Device;
import :Buffer;

namespace Vulkan {
    export class Descriptor {
    public:
        // https://docs.vulkan.org/tutorial/latest/_attachments/22_descriptor_layout.cpp
        Descriptor(Device& device, UniformBufferManager& uniformBufferManager) {
            // createDescriptorSetLayout
            vk::DescriptorSetLayoutBinding uboLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr);
            vk::DescriptorSetLayoutCreateInfo layoutInfo{ .bindingCount = 1, .pBindings = &uboLayoutBinding };
            descriptorSetLayout = vk::raii::DescriptorSetLayout(device.getDevice(), layoutInfo);

            // createDescriptorPool
            vk::DescriptorPoolSize poolSize(vk::DescriptorType::eUniformBuffer, uniformBufferManager.getSize());
            vk::DescriptorPoolCreateInfo poolInfo{ .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, .maxSets = uniformBufferManager.getSize(), .poolSizeCount = 1, .pPoolSizes = &poolSize };
            descriptorPool = vk::raii::DescriptorPool(device.getDevice(), poolInfo);

            // createDescriptorSets
            std::vector<vk::DescriptorSetLayout> layouts(uniformBufferManager.getSize(), *descriptorSetLayout);
            vk::DescriptorSetAllocateInfo allocInfo{ .descriptorPool = descriptorPool, .descriptorSetCount = static_cast<uint32_t>(layouts.size()), .pSetLayouts = layouts.data() };
            descriptorSets = device.getDevice().allocateDescriptorSets(allocInfo);
            for (size_t i = 0; i < uniformBufferManager.getSize(); ++i) {
                vk::DescriptorBufferInfo bufferInfo{ .buffer = uniformBufferManager.getUniformBuffer(i), .offset = 0, .range = sizeof(UniformBufferObject) };
                vk::WriteDescriptorSet descriptorWrite{ .dstSet = descriptorSets[i], .dstBinding = 0, .dstArrayElement = 0, .descriptorCount = 1, .descriptorType = vk::DescriptorType::eUniformBuffer, .pBufferInfo = &bufferInfo };
                device.getDevice().updateDescriptorSets(descriptorWrite, {});
            }
        }

        vk::raii::DescriptorSetLayout& getDescriptorSetLayout() {
            return descriptorSetLayout;
        }

        vk::raii::DescriptorSet& getDescriptorSet(size_t i) {
            return descriptorSets[i];
        }
    private:
        vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
        vk::raii::DescriptorPool descriptorPool = nullptr;
        std::vector<vk::raii::DescriptorSet> descriptorSets;
    };
}