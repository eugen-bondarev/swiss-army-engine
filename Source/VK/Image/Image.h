#ifndef __VK_Image_Image_h__
#define __VK_Image_Image_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(CommandPool);
    FORWARD_DECLARE(Device);    
    FORWARD_DECLARE(Buffer);

    namespace Util
    {
        void TransitionImageLayout(const VkImage& image, const VkImageLayout oldLayout, const VkImageLayout newLayout, const CommandPool& commandPool = GetDefaultCommandPool());
        void CopyBufferToImage(const VkBuffer& buffer, const VkImage& image, const Vec2ui size, const CommandPool& commandPool = GetDefaultCommandPool());
    }

    class Image
    {
    public:
        Image(
            const Vec2ui size,
            const VkFormat format,
            const VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            const VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT,
            const uint32_t mipLevels = 1,
            const Device& device = GetDevice(),
            const CommandPool& commandPool = GetDefaultCommandPool()
        );
        
       ~Image();

        VkFormat GetVkFormat() const;

        void LoadFrom(const Buffer& buffer);

        const VkImage& GetVkImage() const;
        const VkDeviceMemory& GetVkDeviceMemory() const;

    private:
        const CommandPool& commandPool;
        const Device& device;

        VkImage vkImage;
        VkDeviceMemory vkMemory;

        VkFormat vkFormat;
        Vec2ui size;

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;
    };
}

#endif