#ifndef __VK_Image_Image_h__
#define __VK_Image_Image_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(CommandPool);
    FORWARD_DECLARE(Device);    
    FORWARD_DECLARE(Buffer);

    namespace Util
    {
        void TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, const CommandPool* commandPool);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, const CommandPool* commandPool);
    }

    class Image
    {
    public:
        Image(Buffer* buffer, const Vec2ui size, const VkFormat format, const VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const Device* device = nullptr, const CommandPool* commandPool = nullptr);
       ~Image();

        VkFormat GetVkFormat() const;

        const VkImage& GetVkImage() const;
        const VkDeviceMemory& GetVkDeviceMemory() const;

    private:
        const CommandPool& commandPool;
        const Device& device;

        VkImage vkImage;
        VkDeviceMemory vkMemory;
        
        VkFormat vkFormat;

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;
    };
}

#endif