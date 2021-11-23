#pragma once

#include "../Common.h"
#include "../Objects.h"

#include "../memory/buffer.h"

namespace VK
{
    namespace Util
    {
        void TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    }

    class Image
    {
    public:
        Image(Buffer* buffer, const unsigned int width, const unsigned int height, const VkFormat format, const VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const Device* device = nullptr);
        Image(Buffer* buffer, Vec2 size, int amount_of_channels, VkImageUsageFlags usage_flags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const Device* device = nullptr);
        ~Image();

        VkFormat GetVkFormat() const;

        VkImage& GetVkImage();
        VkDeviceMemory& GetVkDeviceMemory();

    private:
        const Device& device;
        VkImage vkImage;
        VkDeviceMemory vkMemory;

        VkFormat vkFormat;

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;
    };
}