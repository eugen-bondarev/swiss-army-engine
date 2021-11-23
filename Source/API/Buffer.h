#ifndef __API_Buffer_h__
#define __API_Buffer_h__

#pragma once

#include "GraphicsContext.h"
#include "../Common/Common.h"

namespace API
{
    class Buffer
    {
    public:
        virtual void* Map() = 0;
        virtual void Unmap() = 0;
    };

    class VertexBuffer : public Buffer
    {
    public:
        static Ptr<VertexBuffer> Create(const unsigned int byteWidth, const unsigned int stride, const void* data);
        virtual void Bind(const unsigned int offset) = 0;
    };

    class IndexBuffer : public Buffer
    {
    public:
        static Ptr<IndexBuffer> Create(const unsigned int byteWidth, const unsigned int stride, const void* data);
        virtual void Bind() = 0;
    };

    class UniformBuffer : public Buffer
    {
    public:
        static Ptr<UniformBuffer> Create(const unsigned int byteWidth, const unsigned int stride, const void* data);
        virtual void Bind() = 0;
    };
}

#endif