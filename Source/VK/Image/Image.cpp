#include "image.h"

#include "../Commands/CommandBuffer.h"
#include "../Commands/CommandPool.h"
#include "../Device/QueueFamily.h"
#include "../Memory/Buffer.h"
#include "../Device/Device.h"

#include "../GraphicsContext.h"

namespace VK
{
    namespace Util
    {			
        void TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, const CommandPool* commandPool)
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
            barrier.subresourceRange.levelCount = 1;
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

        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, const CommandPool* commandPool)
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
            region.imageExtent = {width, height, 1};

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

    Image::Image(Buffer* buffer, const Vec2ui size, const VkFormat format, const VkImageUsageFlags usageFlags, const Device* device, const CommandPool* commandPool) : device{device ? *device : GetDevice()}, commandPool{commandPool ? *commandPool : GetDefaultCommandPool()}, vkFormat{format}
    {
        VkImageCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.imageType = VK_IMAGE_TYPE_2D;
        createInfo.extent.width = static_cast<uint32_t>(size.x);
        createInfo.extent.height = static_cast<uint32_t>(size.y);
        createInfo.extent.depth = 1;
        createInfo.mipLevels = 1;
        createInfo.arrayLayers = 1;
        createInfo.format = format;
        createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        createInfo.usage = usageFlags;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        createInfo.flags = 0;
        VK_TRY(vkCreateImage(this->device.GetVkDevice(), &createInfo, nullptr, &vkImage));

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(this->device.GetVkDevice(), vkImage, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = this->device.FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        VK_TRY(vkAllocateMemory(this->device.GetVkDevice(), &allocInfo, nullptr, &vkMemory));

        vkBindImageMemory(this->device.GetVkDevice(), vkImage, vkMemory, 0);

        if (buffer != nullptr)
        {
            Util::TransitionImageLayout(vkImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &this->commandPool);
            Util::CopyBufferToImage(buffer->GetVkBuffer(), vkImage, static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y), &this->commandPool);
            Util::TransitionImageLayout(vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &this->commandPool);
        }
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