#ifndef __VK_Memory_EntityUniformBuffer_h__
#define __VK_Memory_EntityUniformBuffer_h__

#pragma once

#include "../../Util/Aligned.h"
#include "Buffer.h"

namespace VK
{
    template <typename T>
    class EntityUniformBuffer
    {
    public:
        EntityUniformBuffer(const size_t numInstances) : data{numInstances}
        {
            buffer = CreatePtr<Buffer>(numInstances * data.GetSize(), 1, data.GetPtr(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
		    SetDescriptor(data.GetSize());
        }

        Aligned<T>& operator()()
        {
            return data.data;
        }

        const Aligned<T>& operator()() const
        {
            return data.data;
        }

        void Update(const void* data, const uint32_t size) const
        {
            buffer->Update(data, size);
        }

        void Update(const void* data) const
        {
            buffer->Update(data);
        }

        uint32_t GetSize() const
        {
            return buffer->GetSize();
        }

        uint32_t GetSizeOfElement() const
        {
            return buffer->GetSizeOfElement();
        }

        uint32_t GetAmountOfElements() const
        {
            return buffer->GetAmountOfElements();
        }

        const VkBuffer& GetVkBuffer() const
        {
            return buffer->GetVkBuffer();
        }

        const VkDeviceMemory& GetVkMemory() const
        {
            return buffer->GetVkMemory();
        }

        const VkDescriptorBufferInfo& GetVkDescriptor() const
        {
            return buffer->GetVkDescriptor();
        }

        void SetDescriptor(const VkDeviceSize range, const VkDeviceSize offset = 0)
        {
            buffer->SetDescriptor(range, offset);
        }

        void Overwrite()
        {
            Update(data.GetPtr());
        }

    private:
        AlignedUBO<T> data;
        Ptr<Buffer> buffer;
    };
}

#endif