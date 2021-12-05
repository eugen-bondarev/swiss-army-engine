#include "Image.h"

#include "../Commands/CommandBuffer.h"
#include "../Commands/CommandPool.h"
#include "../Device/QueueFamily.h"
#include "../GraphicsContext.h"
#include "../Memory/Buffer.h"
#include "../Device/Device.h"

namespace VK
{
    static void GenerateMipmaps(
        const VkImage& image, 
        const int32_t width, 
        const int32_t height, 
        const uint32_t mipLevels, 
        const CommandPool& commandPool = GetDefaultCommandPool()
    )
    {
        CommandBuffer commandBuffer(commandPool);
        commandBuffer.Begin();
        
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        int32_t mipWidth = width;
        int32_t mipHeight = height;

        for (uint32_t i = 1; i < mipLevels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer.GetVkCommandBuffer(),
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer.GetVkCommandBuffer(),
                image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer.GetVkCommandBuffer(),
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer.GetVkCommandBuffer(),
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        commandBuffer.End();

        commandBuffer.SubmitToQueue(Queues::graphicsQueue);
        vkQueueWaitIdle(Queues::graphicsQueue);
    }

    namespace Util
    {
        void TransitionImageLayout(
            const VkImage& image,
            const VkImageLayout oldLayout,
            const VkImageLayout newLayout,
            const uint32_t mipLevels,
            const CommandPool& commandPool
        )
        {
            CommandBuffer commandBuffer(commandPool);
            
            commandBuffer.Begin();

            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = mipLevels;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            // barrier.srcAccessMask = 0; // TODO
            // barrier.dstAccessMask = 0; // TODO

            VkPipelineStageFlags srcStage;
            VkPipelineStageFlags dstStage;

            if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
            {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            }
            else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
            {
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            } 
            else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) 
            {
                barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

                srcStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            } 
            else 
            {
                throw EXCEPTION_WHAT("Unsupported layout transition.");
            }

            vkCmdPipelineBarrier(
                commandBuffer.GetVkCommandBuffer(),
                srcStage, dstStage,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );

            commandBuffer.End();
            commandBuffer.SubmitToQueue(Queues::graphicsQueue);
            vkQueueWaitIdle(Queues::graphicsQueue);
        }

        void CopyBufferToImage(const VkBuffer& buffer, const VkImage& image, const Vec2ui size, const CommandPool& commandPool)
        {
            CommandBuffer commandBuffer(commandPool);
            
            commandBuffer.Begin();

            VkBufferImageCopy region{};
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;

            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;

            region.imageOffset = {0, 0, 0};
            region.imageExtent = {size.x, size.y, 1};

            vkCmdCopyBufferToImage(
                commandBuffer.GetVkCommandBuffer(),
                buffer,
                image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &region
            );

            commandBuffer.End();
            commandBuffer.SubmitToQueue(Queues::graphicsQueue);
            vkQueueWaitIdle(Queues::graphicsQueue);
        }
    }

    Image::Image(const Vec2ui size, const VkFormat format, const VkImageUsageFlags usageFlags, const VkSampleCountFlagBits numSamples, const uint32_t mipLevels, const Device& device, const CommandPool& commandPool) : device{device}, commandPool{commandPool}, vkFormat{format}, size{size}
    {
        VkImageCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.imageType = VK_IMAGE_TYPE_2D;
        createInfo.extent.width = static_cast<uint32_t>(size.x);
        createInfo.extent.height = static_cast<uint32_t>(size.y);
        createInfo.extent.depth = 1;
        createInfo.mipLevels = mipLevels;
        createInfo.arrayLayers = 1;
        createInfo.format = format;
        createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        createInfo.usage = usageFlags;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.samples = numSamples;
        createInfo.flags = 0;
        VK_TRY(vkCreateImage(device.GetVkDevice(), &createInfo, nullptr, &vkImage));

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device.GetVkDevice(), vkImage, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = device.FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        VK_TRY(vkAllocateMemory(device.GetVkDevice(), &allocInfo, nullptr, &vkMemory));

        vkBindImageMemory(device.GetVkDevice(), vkImage, vkMemory, 0);
    }
        
    void Image::LoadFrom(const Buffer& buffer, const uint32_t mipLevels)
    {
        Util::TransitionImageLayout(vkImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels, this->commandPool);
        Util::CopyBufferToImage(buffer.GetVkBuffer(), vkImage, size, this->commandPool);
        GenerateMipmaps(vkImage, static_cast<int32_t>(size.x), static_cast<int32_t>(size.y), mipLevels, this->commandPool);
        // Util::TransitionImageLayout(vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels, this->commandPool);

    }

    Image::~Image()
    {
        vkDestroyImage(device.GetVkDevice(), vkImage, nullptr);
        vkFreeMemory(device.GetVkDevice(), vkMemory, nullptr);
    }

    const VkImage& Image::GetVkImage() const
    {
        return vkImage;
    }

    const VkDeviceMemory& Image::GetVkDeviceMemory() const
    {
        return vkMemory;
    }

    VkFormat Image::GetVkFormat() const
    {
        return vkFormat;
    }
}