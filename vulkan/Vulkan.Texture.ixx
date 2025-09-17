#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// https://docs.vulkan.org/tutorial/latest/_attachments/24_texture_image.cpp
// からコピペ

export module Vulkan:Texture;



import std;


import vulkan_hpp;
import :Settings;
import :Device;
import :Buffer;
import :Rendering;
import :Image;


namespace Vulkan {
    export class Texture {
    public:
        Texture(Device& device, Rendering& rendering, std::string_view filename) {
            int texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load(filename.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            vk::DeviceSize imageSize = texWidth * texHeight * 4;

            if (!pixels) {
                throw std::runtime_error("failed to load texture image!");
            }

            vk::raii::Buffer stagingBuffer({});
            vk::raii::DeviceMemory stagingBufferMemory({});
            Buffer::createBuffer(device, imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

            void* data = stagingBufferMemory.mapMemory(0, imageSize);
            std::memcpy(data, pixels, imageSize);
            stagingBufferMemory.unmapMemory();

            stbi_image_free(pixels);

            Image::createImage(device, texWidth, texHeight, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);

            transitionImageLayout(rendering, textureImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
            copyBufferToImage(rendering, stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
            transitionImageLayout(rendering, textureImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

            textureImageView = Image::createImageView(device, textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);
            textureSampler = createTextureSampler(device);
        }


        // 本当はSamplerCreateInfoの初期設定をするBuilderを用意するべき
        static vk::raii::Sampler createTextureSampler(Device& device) {
            vk::PhysicalDeviceProperties properties = device.getPhysicalDevice().getProperties();
            vk::SamplerCreateInfo samplerInfo{
                // magFilterとフィールドminFilterは、拡大または縮小されたテクセルをどのように補間するか
                // https://docs.vulkan.org/tutorial/latest/06_Texture_mapping/01_Image_view_and_sampler.html#_samplers
                .magFilter = vk::Filter::eLinear,
                .minFilter = vk::Filter::eLinear,
                .mipmapMode = vk::SamplerMipmapMode::eLinear,
                .addressModeU = vk::SamplerAddressMode::eRepeat,
                .addressModeV = vk::SamplerAddressMode::eRepeat,
                .addressModeW = vk::SamplerAddressMode::eRepeat,
                .mipLodBias = 0.0f,
                .anisotropyEnable = vk::True,

                // 最終的な色の計算に使用できるテクセルサンプルの数を制限
                // 値が小さいほどパフォーマンスは向上するが、結果の品質は低下
                .maxAnisotropy = properties.limits.maxSamplerAnisotropy,

                .compareEnable = vk::False,
                .compareOp = vk::CompareOp::eAlways };
            return vk::raii::Sampler(device.getDevice(), samplerInfo);
        }

        


        vk::raii::ImageView& getTextureImageView() {
            return textureImageView;
        }

        vk::raii::Sampler& getTextureSampler() {
            return textureSampler;
        }

    private:
        vk::raii::Image textureImage = nullptr;
        vk::raii::DeviceMemory textureImageMemory = nullptr;
        vk::raii::ImageView textureImageView = nullptr;
        vk::raii::Sampler textureSampler = nullptr;

        void transitionImageLayout(Rendering& rendering, const vk::raii::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
            auto commandBuffer = rendering.beginSingleTimeCommands();

            vk::ImageMemoryBarrier barrier{
                .oldLayout = oldLayout,
                .newLayout = newLayout,
                .image = image,
                // .subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 }
                .subresourceRange = Settings::defaultImageSubresourceRange()
            };

            vk::PipelineStageFlags sourceStage;
            vk::PipelineStageFlags destinationStage;

            if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
                barrier.srcAccessMask = {};
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

                sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
                destinationStage = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

                sourceStage = vk::PipelineStageFlagBits::eTransfer;
                destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else {
                throw std::invalid_argument("unsupported layout transition!");
            }
            commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, {}, nullptr, barrier);
            rendering.endSingleTimeCommands(commandBuffer);
        }

        void copyBufferToImage(Rendering& rendering, const vk::raii::Buffer& buffer, vk::raii::Image& image, uint32_t width, uint32_t height) {
            auto commandBuffer = rendering.beginSingleTimeCommands();
            vk::BufferImageCopy region{
                .bufferOffset = 0,
                .bufferRowLength = 0,
                .bufferImageHeight = 0,
                // Settingsにdefault追加した方が良いかは不明
                .imageSubresource = { vk::ImageAspectFlagBits::eColor, 0, 0, 1 },

                .imageExtent = {width, height, 1}
            };
            commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, { region });
            rendering.endSingleTimeCommands(commandBuffer);
        }
    };
}