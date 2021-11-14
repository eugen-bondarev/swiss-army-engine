#ifndef __D3D_BUFFER_H__
#define __D3D_BUFFER_H__

#pragma once

#include "common.h"

class Buffer
{
public:
    Buffer(const UINT byteWidth, const UINT stride, const void* data = nullptr, const UINT bindFlags = D3D11_BIND_VERTEX_BUFFER)
    {    
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.BindFlags = bindFlags;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = byteWidth;
        bufferDesc.StructureByteStride = stride;

        D3D11_SUBRESOURCE_DATA bufferSubData{};

        if (data)
        {
            bufferSubData.pSysMem = data;
        }

        D3D_CHECK(common::device->CreateBuffer(&bufferDesc, &bufferSubData, &dxBuffer));
    }

   ~Buffer()
    {
    }

protected:
    ComPtr<ID3D11Buffer> dxBuffer;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
};

class VertexBuffer : public Buffer
{
public:
    VertexBuffer(const UINT byteWidth, const UINT stride, const void* data = nullptr) : Buffer(byteWidth, stride, data, D3D11_BIND_VERTEX_BUFFER)
    {
    }

    void Bind(const UINT stride, const UINT offset)
    {
        common::ctx->IASetVertexBuffers(0u, 1u, dxBuffer.GetAddressOf(), &stride, &offset);
    }
};

class IndexBuffer : public Buffer
{
public:
    IndexBuffer(const UINT byteWidth, const UINT stride, const void* data = nullptr) : Buffer(byteWidth, stride, data, D3D11_BIND_INDEX_BUFFER)
    {
    }

    void Bind()
    {
        common::ctx->IASetIndexBuffer(dxBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
    }
};

#endif