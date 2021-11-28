#ifndef __VK_Memory_VertexBuffer_h__
#define __VK_Memory_VertexBuffer_h__

#pragma once

#include "Buffer.h"

namespace VK
{
    class VertexBuffer
    {
    public:
        template <typename T>
        VertexBuffer(const std::vector<T>& vector, const Device& device = GetDevice())
        {
            Buffer stagingBuffer(vector);
            buffer = CreatePtr<Buffer>(stagingBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        }

        Buffer* UnderlyingPtr()
        {
            return buffer.get();
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