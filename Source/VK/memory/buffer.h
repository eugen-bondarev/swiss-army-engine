#pragma once

#include "../Common.h"
#include "../Objects.h"

#include "../commands/command_buffer.h"
#include "../commands/command_pool.h"

#include "../GraphicsContext.h"

namespace VK
{
    namespace Util
    {
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const Device& device = GetDevice());
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const Device& device = GetDevice());
    }

    class Buffer
    {
    public:
        Buffer(
            uint32_t size_of_element, 
            uint32_t amount_of_elements = 0, 
            const void* data = nullptr, 
            VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            const Device* device = nullptr
        );
        
        template <typename T>
        Buffer(
            const std::vector<T>& vector, 
            VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            const Device* device = nullptr
        ) : Buffer(sizeof(T), static_cast<uint32_t>(vector.size()), vector.data(), usage_flags, property_flags, device)
        {
            
        }

        Buffer(
            const void* data,
            uint32_t size,
            VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            const Device* device = nullptr
        ) : Buffer(size, 1, data, usage_flags, property_flags, device)
        {
            
        }

        Buffer(Buffer* buffer, VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, const Device* device = nullptr, const CommandPool* commandPool = &GetDefaultCommandPool());
        ~Buffer();

        void Update(const void* data, uint32_t size) const;
        void Update(const void* data) const;

        uint32_t GetSize() const;
        uint32_t GetSizeOfElement() const;
        uint32_t GetAmountOfElements() const;

        VkBuffer& GetVkBuffer();
        VkDeviceMemory& GetVkMemory();

        VkDescriptorBufferInfo& GetDescriptor();
        void SetDescriptor(VkDeviceSize range, VkDeviceSize offset = 0);

    private:
        const Device& device;

        VkBuffer vkBuffer;
        VkDeviceMemory vkMemory;

        VkDescriptorBufferInfo descriptor;
        void SetupDefaultDescriptor();

        uint32_t sizeOfElement;
        uint32_t amountOfElements;

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
    };
}