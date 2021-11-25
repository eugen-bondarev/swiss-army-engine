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

    private:
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

    template <typename T>
    class SceneUniformBuffer : public Buffer
    {
    public:
        SceneUniformBuffer() : Buffer(sizeof(T), 1, &data, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        {
        }

        T& operator()()
        {
            return data;
        }

        const T& operator()() const
        {
            return data;
        }

        void Overwrite()
        {
            Update(&data);
        }

    private:
        T data;
    };

    template <typename T>
    class EntityUniformBuffer : public Buffer
    {
    public:
        EntityUniformBuffer(const size_t numInstances) : data{numInstances}, Buffer(numInstances * data.GetSize(), 1, &data, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        {
		    SetDescriptor(data.GetSize());
        }

        T& operator()()
        {
            return data;
        }

        const T& operator()() const
        {
            return data;
        }

        void Overwrite()
        {
            Update(data.GetPtr());
        }

        T data;
    private:
    };
}

#endif