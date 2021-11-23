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
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& buffer_memory, const Device& device)
        {			
            VkBufferCreateInfo buffer_info{};
            buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_info.size = size;
            buffer_info.usage = usage;
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VK_TRY(vkCreateBuffer(device.GetVkDevice(), &buffer_info, nullptr, &buffer));

            VkMemoryRequirements mem_requirements;
            vkGetBufferMemoryRequirements(device.GetVkDevice(), buffer, &mem_requirements);

            VkMemoryAllocateInfo alloc_info{};
            alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            alloc_info.allocationSize = mem_requirements.size;
            alloc_info.memoryTypeIndex = device.FindMemoryType(mem_requirements.memoryTypeBits, properties);

            VK_TRY(vkAllocateMemory(device.GetVkDevice(), &alloc_info, nullptr, &buffer_memory));

            vkBindBufferMemory(device.GetVkDevice(), buffer, buffer_memory, 0);
        }
    }

    void Buffer::SetupDefaultDescriptor()
    {
        descriptor.buffer = vkBuffer;
        descriptor.offset = 0;
        descriptor.range = VK_WHOLE_SIZE;
    }

    void Buffer::SetDescriptor(VkDeviceSize range, VkDeviceSize offset)
    {
        descriptor.offset = offset;
        descriptor.range = range;
    }

    Buffer::Buffer(uint32_t size_of_element, uint32_t amount_of_elements, const void* data, VkBufferUsageFlags usage_flags, VkMemoryPropertyFlags property_flags, const Device* device) : device{device ? *device : GetDevice()}, sizeOfElement{size_of_element}, amountOfElements{amount_of_elements}
    {
        VkDeviceSize buffer_size = size_of_element * amount_of_elements;
        
        Util::CreateBuffer(
            buffer_size, 
            usage_flags, 
            property_flags, 
            vkBuffer, 
            vkMemory
        );

        if (data != nullptr)
        {
            void* mapped_data;
            vkMapMemory(this->device.GetVkDevice(), vkMemory, 0, buffer_size, 0, &mapped_data);
                memcpy(mapped_data, data, static_cast<size_t>(buffer_size));
            vkUnmapMemory(this->device.GetVkDevice(), vkMemory);
        }

        SetupDefaultDescriptor();
    }

    Buffer::Buffer(Buffer* buffer, VkBufferUsageFlags usage_flags, const Device* device, const CommandPool* commandPool) : device{device ? *device : GetDevice()}
    {
        amountOfElements = buffer->amountOfElements;
        sizeOfElement = buffer->sizeOfElement;

        Util::CreateBuffer(
            buffer->GetSize(), 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage_flags, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            vkBuffer, 
            vkMemory
        );

        CommandBuffer command_buffer(commandPool);
            command_buffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
                VkBufferCopy copy_region{};
                copy_region.size = buffer->GetSize();
                vkCmdCopyBuffer(command_buffer.GetVkCommandBuffer(), buffer->GetVkBuffer(), vkBuffer, 1, &copy_region);			
            command_buffer.End();
        command_buffer.SubmitToQueue(Queues::graphicsQueue);
        this->device.WaitIdle();

        SetupDefaultDescriptor();
    }

    void Buffer::Update(const void* data, uint32_t size) const
    {
        void* mapped_data;
        vkMapMemory(device.GetVkDevice(), vkMemory, 0, size, 0, &mapped_data);
            memcpy(mapped_data, data, size);

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
        void* mapped_data;
        vkMapMemory(device.GetVkDevice(), vkMemory, 0, sizeOfElement * amountOfElements, 0, &mapped_data);
            memcpy(mapped_data, data, sizeOfElement * amountOfElements);
        vkUnmapMemory(device.GetVkDevice(), vkMemory);
    }

    Buffer::~Buffer()
    {
        vkDestroyBuffer(device.GetVkDevice(), vkBuffer, nullptr);
        vkFreeMemory(device.GetVkDevice(), vkMemory, nullptr);
    }

    VkBuffer& Buffer::GetVkBuffer()
    {
        return vkBuffer;
    }

    VkDeviceMemory& Buffer::GetVkMemory()
    {
        return vkMemory;
    }

    VkDescriptorBufferInfo& Buffer::GetDescriptor()
    {
        return descriptor;
    }

    uint32_t Buffer::GetSize() const
    {
        return sizeOfElement * amountOfElements;
    }

    uint32_t Buffer::GetSizeOfElement() const
    {
        return sizeOfElement;
    }

    uint32_t Buffer::GetAmountOfElements() const
    {
        return amountOfElements;
    }
}