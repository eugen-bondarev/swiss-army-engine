#ifndef __VK_Memory_Buffer_h__
#define __VK_Memory_Buffer_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(CommandPool);
    FORWARD_DECLARE(CommandBuffer);

    namespace Util
    {
        void CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const Device& device = GetDevice());
    }

    class Buffer
    {
    public:
        Buffer(
            const uint32_t elementSize, 
            const uint32_t numElements = 0, 
            const void* data = nullptr, 
            const VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            const VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            const Device& device = GetDevice()
        );
        
        template <typename T>
        Buffer(
            const std::vector<T>& vector, 
            const VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            const VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            const Device& device = GetDevice()
        ) : Buffer(sizeof(T), static_cast<uint32_t>(vector.size()), vector.data(), usageFlags, propertyFlags, device) 
        {
        }

        Buffer(
            const void* data,
            const uint32_t size,
            const VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            const VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            const Device& device = GetDevice()
        ) : Buffer(size, 1, data, usageFlags, propertyFlags, device)
        {   
        }

        Buffer(const Buffer& buffer, const VkBufferUsageFlags usageFlags, const Device& device = GetDevice(), const CommandPool& commandPool = GetDefaultCommandPool());
       ~Buffer();

        void Update(const void* data, const uint32_t size) const;
        void Update(const void* data) const;

        uint32_t GetSize() const;
        uint32_t GetSizeOfElement() const;
        uint32_t GetAmountOfElements() const;

        const VkBuffer& GetVkBuffer() const;
        const VkDeviceMemory& GetVkMemory() const;

        const VkDescriptorBufferInfo& GetVkDescriptor() const;
        void SetDescriptor(const VkDeviceSize range, const VkDeviceSize offset = 0);

    protected:
        const Device& device;

        VkBuffer vkBuffer;
        VkDeviceMemory vkMemory;
        VkDescriptorBufferInfo vkDescriptor;

        uint32_t elementSize;
        uint32_t numElements;

        void SetupDefaultDescriptor();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
    };
}

#endif