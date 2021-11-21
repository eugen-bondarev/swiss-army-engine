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
        Buffer(const uint32_t bufferSize, const uint32_t stride, const void* data = nullptr, const VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT, const VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
       ~Buffer() = default;

        void* Map();
        void Unmap();

        const VkBuffer& GetVkBuffer() const;
        const VkDeviceMemory& GetVkDeviceMemory() const;
        const VkDescriptorBufferInfo& GetVkDescriptor() const;

    protected:
        VkBuffer vkBuffer;
        VkDeviceMemory vkMemory;

        VkDescriptorBufferInfo vkDescriptor;
        void SetupDefaultDescriptor();

        uint32_t stride;

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
    };

    class VertexBuffer : public API::VertexBuffer, public Buffer
    {
    public:
        VertexBuffer(const uint32_t bufferSize, const uint32_t stride, const void* data = nullptr);
        void Bind(const unsigned int offset) override;
        void* Map() override;
        void Unmap() override;
    };

    class IndexBuffer : public API::IndexBuffer, public Buffer
    {
    public:
        IndexBuffer(const uint32_t bufferSize, const uint32_t stride, const void* data = nullptr);
        void Bind() override;
        void* Map() override;
        void Unmap() override;
    };
}

#endif