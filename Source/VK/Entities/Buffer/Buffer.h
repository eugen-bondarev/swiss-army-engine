#ifndef __VK_Entities_Buffer_Buffer_h__
#define __VK_Entities_Buffer_Buffer_h__

#pragma once

#include "../../../API/Buffer.h"

namespace VK
{
    namespace Util
    {
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    }

    class Buffer
    {
    public:
        Buffer(
            const uint32_t bufferSize,
            const void* data = nullptr, 
            const VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            const VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        ~Buffer() = default;

        void* Map();
        void Unmap();

    protected:
        VkBuffer vkBuffer;
        VkDeviceMemory vkMemory;

        VkDescriptorBufferInfo descriptor;
        void SetupDefaultDescriptor();

        unsigned int stride;

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
    };

    class VertexBuffer
    {

    };
}

#endif