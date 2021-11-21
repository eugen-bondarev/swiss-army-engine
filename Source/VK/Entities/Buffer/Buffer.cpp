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
        vkDescriptor.buffer = vkBuffer;
        vkDescriptor.offset = 0;
        vkDescriptor.range = VK_WHOLE_SIZE;
    }

    Buffer::Buffer(const uint32_t bufferSize, const uint32_t stride, const void* data,  const VkBufferUsageFlags usageFlags, const VkMemoryPropertyFlags propertyFlags) : stride{stride}
    {
        Util::CreateBuffer(bufferSize, usageFlags, propertyFlags, vkBuffer, vkMemory);

        if (data != nullptr)
        {
            void* mappedData;
            vkMapMemory(GetDevice()->GetVkDevice(), vkMemory, 0, bufferSize, 0, &mappedData);
                memcpy(mappedData, data, static_cast<size_t>(bufferSize));
            vkUnmapMemory(GetDevice()->GetVkDevice(), vkMemory);
        }

        SetupDefaultDescriptor();
    }

    const VkBuffer& Buffer::GetVkBuffer() const
    {
        return vkBuffer;
    }

    const VkDeviceMemory& Buffer::GetVkDeviceMemory() const
    {
        return vkMemory;
    }

    const VkDescriptorBufferInfo& Buffer::GetVkDescriptor() const
    {
        return vkDescriptor;
    }


    
    VertexBuffer::VertexBuffer(const uint32_t bufferSize, const uint32_t stride, const void* data) : ::VK::Buffer(bufferSize, stride, data, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
    }

    void VertexBuffer::Bind(const unsigned int offset)
    {
    }

    void* VertexBuffer::Map()
    {
        return nullptr;
    }

    void VertexBuffer::Unmap()
    {

    }



    IndexBuffer::IndexBuffer(const uint32_t bufferSize, const uint32_t stride, const void* data) : ::VK::Buffer(bufferSize, stride, data, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
    }

    void IndexBuffer::Bind()
    {
    }

    void* IndexBuffer::Map()
    {
        return nullptr;
    }

    void IndexBuffer::Unmap()
    {

    }
}