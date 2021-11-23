#pragma once

#include "../Common.h"
#include "../Objects.h"

#include "../memory/buffer.h"

namespace VK
{
    FORWARD_DECLARE(CommandPool);

    namespace Util
    {
        void TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, const CommandPool* commandPool);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, const CommandPool* commandPool);
    }

    class Image
    {
    public:
        Image(Buffer* buffer, const unsigned int width, const unsigned int height, const VkFormat format, const VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const Device* device = nullptr, const CommandPool* commandPool = nullptr);
        Image(Buffer* buffer, Vec2 size, int amount_of_channels, VkImageUsageFlags usage_flags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, const Device* device = nullptr, const CommandPool* commandPool = nullptr);
        ~Image();

        VkFormat GetVkFormat() const;

        VkImage& GetVkImage();
        VkDeviceMemory& GetVkDeviceMemory();

    private:
        const Device& device;
        const CommandPool& commandPool;
        VkImage vkImage;
        VkDeviceMemory vkMemory;

        VkFormat vkFormat;

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;
    };
}