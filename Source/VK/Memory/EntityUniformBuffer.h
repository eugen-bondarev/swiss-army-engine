#ifndef __VK_Memory_EntityUniformBuffer_h__
#define __VK_Memory_EntityUniformBuffer_h__

#pragma once

#include "../../Util/Aligned.h"
#include "Buffer.h"

namespace VK
{
    template <typename T>
    class EntityUniformBuffer : public Buffer
    {
    public:
        EntityUniformBuffer(const size_t numInstances) : data{numInstances}, Buffer(numInstances * data.GetSize(), 1, &data, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        {
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

        void Overwrite()
        {
            Update(data.GetPtr());
        }

    private:
        AlignedUBO<T> data;
    };
}

#endif