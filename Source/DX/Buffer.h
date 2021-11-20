#ifndef __DX_Buffer_h__
#define __DX_Buffer_h__

#pragma once

#include "Instance.h"

#include "../API/Buffer.h"

namespace DX
{
    class Buffer
    {
    public:
        Buffer(const UINT byteWidth, const UINT stride, const void* data, const UINT bindFlags, const UINT cpuAccessFlags = 0u, const D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
        ~Buffer() = default;

        void* Map();
        void Unmap();

    protected:
        unsigned int stride;
        ComPtr<ID3D11Buffer> dxBuffer;

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
    };

    class VertexBuffer : public API::VertexBuffer, public Buffer
    {
    public:
        VertexBuffer(const UINT byteWidth, const UINT stride = 0u, const void* data = nullptr);

        void Bind(const UINT offset) override;
        void* Map() override;
        void Unmap() override;
    };

    class IndexBuffer : public API::IndexBuffer, public Buffer
    {
    public:
        IndexBuffer(const UINT byteWidth, const UINT stride = 0u, const void* data = nullptr);

        void Bind() override;
        void* Map() override;
        void Unmap() override;
    };

    class ConstantBuffer : public API::UniformBuffer, public Buffer
    {
    public:
        ConstantBuffer(const UINT byteWidth, const UINT stride = 0u, const void* data = nullptr);

        void Bind() override;
        void* Map() override;
        void Unmap() override;
    };
}

#endif