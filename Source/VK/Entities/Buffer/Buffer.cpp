#include "Buffer.h"

#include "../../GraphicsContext.h"

namespace VK
{
    namespace Util
    {
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
        {
            VkBufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.size = size;
            createInfo.usage = usage;
            createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VK_TRY(vkCreateBuffer(GetDevice()->GetVkDevice(), &createInfo, nullptr, &buffer));

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(GetDevice()->GetVkDevice(), buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = GetDevice()->FindMemoryType(memRequirements.memoryTypeBits, properties);

            VK_TRY(vkAllocateMemory(GetDevice()->GetVkDevice(), &allocInfo, nullptr, &bufferMemory));

            vkBindBufferMemory(GetDevice()->GetVkDevice(), buffer, bufferMemory, 0);
        }
    }

    void Buffer::SetupDefaultDescriptor()
    {
        descriptor.buffer = vkBuffer;
        descriptor.offset = 0;
        descriptor.range = VK_WHOLE_SIZE;
    }

    Buffer::Buffer(
            const uint32_t bufferSize,
            const void* data, 
            const VkBufferUsageFlags usageFlags,
            const VkMemoryPropertyFlags propertyFlags
        )
    {			
        Util::CreateBuffer(
            bufferSize, 
            usageFlags, 
            propertyFlags, 
            vkBuffer, 
            vkMemory
        );

        if (data != nullptr)
        {
            void* mappedData;
            vkMapMemory(GetDevice()->GetVkDevice(), vkMemory, 0, bufferSize, 0, &mappedData);
                memcpy(mappedData, data, static_cast<size_t>(bufferSize));
            vkUnmapMemory(GetDevice()->GetVkDevice(), vkMemory);
        }

        SetupDefaultDescriptor();
    }
}