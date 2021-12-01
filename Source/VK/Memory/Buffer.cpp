#include "Buffer.h"

#include "../Commands/CommandBuffer.h"
#include "../Commands/CommandPool.h"
#include "../Device/QueueFamily.h"
#include "../GraphicsContext.h"
#include "../Device/Device.h"

namespace VK
{
    namespace Util 
    {
        void CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const Device& device)
        {			
            VkBufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.size = size;
            createInfo.usage = usage;
            createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            VK_TRY(vkCreateBuffer(device.GetVkDevice(), &createInfo, nullptr, &buffer));

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(device.GetVkDevice(), buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = device.FindMemoryType(memRequirements.memoryTypeBits, properties);
            VK_TRY(vkAllocateMemory(device.GetVkDevice(), &allocInfo, nullptr, &bufferMemory));

            vkBindBufferMemory(device.GetVkDevice(), buffer, bufferMemory, 0);
        }
    }

    void Buffer::SetupDefaultDescriptor()
    {
        vkDescriptor.buffer = vkBuffer;
        vkDescriptor.offset = 0;
        vkDescriptor.range = VK_WHOLE_SIZE;
    }

    void Buffer::SetDescriptor(const VkDeviceSize range, const VkDeviceSize offset)
    {
        vkDescriptor.offset = offset;
        vkDescriptor.range = range;
    }

    Buffer::Buffer(const uint32_t elementSize, const uint32_t numElements, const void* data, const VkBufferUsageFlags usageFlags, const VkMemoryPropertyFlags propertyFlags, const Device& device) : device{device}, elementSize{elementSize}, numElements{numElements}
    {
        const VkDeviceSize bufferSize{elementSize * numElements};
        
        Util::CreateBuffer(
            bufferSize, 
            usageFlags, 
            propertyFlags, 
            vkBuffer, 
            vkMemory
        );

        if (data != nullptr)
        {
            Update(data);
        }

        SetupDefaultDescriptor();
    }

    Buffer::Buffer(
        const Buffer& buffer, 
        const VkBufferUsageFlags usageFlags, 
        const Device& device, 
        const CommandPool& commandPool
    ) : device{device}, numElements{buffer.numElements}, elementSize{buffer.elementSize}
    {
        Util::CreateBuffer(
            buffer.GetSize(),
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | usageFlags,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            vkBuffer, 
            vkMemory
        );

        CommandBuffer commandBuffer(commandPool);
            commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
                VkBufferCopy copyRegion{};
                copyRegion.size = buffer.GetSize();
                vkCmdCopyBuffer(commandBuffer.GetVkCommandBuffer(), buffer.GetVkBuffer(), vkBuffer, 1, &copyRegion);			
            commandBuffer.End();
        commandBuffer.SubmitToQueue(Queues::graphicsQueue);
        this->device.WaitIdle();

        SetupDefaultDescriptor();
    }

    void Buffer::Update(const void* data, const uint32_t size) const
    {
        void* mappedData;
        vkMapMemory(device.GetVkDevice(), vkMemory, 0, size, 0, &mappedData);
            memcpy(mappedData, data, size);

        // VkMappedMemoryRange range;
        // range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        // range.memory = vkMemory;
        // range.size = VK_WHOLE_SIZE;
        // range.offset = 0;
        // vkFlushMappedMemoryRanges(device.GetVkDevice(), 1, &range);
        
        vkUnmapMemory(device.GetVkDevice(), vkMemory);
    }

    void Buffer::Update(const void* data) const
    {
        void* mappedData;
        vkMapMemory(device.GetVkDevice(), vkMemory, 0, elementSize * numElements, 0, &mappedData);
            memcpy(mappedData, data, elementSize * numElements);
        vkUnmapMemory(device.GetVkDevice(), vkMemory);
    }

    Buffer::~Buffer()
    {
        vkDestroyBuffer(device.GetVkDevice(), vkBuffer, nullptr);
        vkFreeMemory(device.GetVkDevice(), vkMemory, nullptr);
    }

    const VkBuffer& Buffer::GetVkBuffer() const
    {
        return vkBuffer;
    }

    const VkDeviceMemory& Buffer::GetVkMemory() const
    {
        return vkMemory;
    }

    const VkDescriptorBufferInfo& Buffer::GetVkDescriptor() const
    {
        return vkDescriptor;
    }

    uint32_t Buffer::GetSize() const
    {
        return elementSize * numElements;
    }

    uint32_t Buffer::GetSizeOfElement() const
    {
        return elementSize;
    }

    uint32_t Buffer::GetAmountOfElements() const
    {
        return numElements;
    }
}