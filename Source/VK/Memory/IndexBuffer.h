#ifndef __VK_Memory_IndexBuffer_h__
#define __VK_Memory_IndexBuffer_h__

#pragma once

#include "Buffer.h"

namespace VK
{
    class IndexBuffer
    {
    public:
        template <typename T>
        IndexBuffer(const std::vector<T>& vector, const Device& device = GetDevice())
        {
            Buffer stagingBuffer(vector);
            buffer = CreatePtr<Buffer>(stagingBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        }

        Buffer* UnderlyingPtr()
        {
            return buffer.get();
        }

        Buffer& UnderlyingRef()
        {
            return *buffer;
        }

        Buffer& Resolve()
        {
            return *buffer;
        }

        Buffer& operator()()
        {
            return Resolve();
        }

    private:
        Ptr<Buffer> buffer;
    };
}

#endif