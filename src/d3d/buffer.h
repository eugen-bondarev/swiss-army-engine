#ifndef __D3D_BUFFER_H__
#define __D3D_BUFFER_H__

#pragma once

#include "common.h"

class Buffer
{
public:
    Buffer(const UINT byteWidth, const UINT stride, const void* data, const UINT bindFlags = D3D11_BIND_VERTEX_BUFFER)
    {    
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.BindFlags = bindFlags;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = byteWidth;
        bufferDesc.StructureByteStride = stride;

        D3D11_SUBRESOURCE_DATA bufferSubData{};
        bufferSubData.pSysMem = data;

        D3D_CHECK(common::device->CreateBuffer(&bufferDesc, &bufferSubData, &dxBuffer));
    }

    void BindVertexBuffer(const UINT stride, const UINT offset)
    {
        common::ctx->IASetVertexBuffers(0u, 1u, dxBuffer.GetAddressOf(), &stride, &offset);
    }

    void BindIndexBuffer()
    {
        common::ctx->IASetIndexBuffer(dxBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
    }

   ~Buffer()
    {

    }

private:
    ComPtr<ID3D11Buffer> dxBuffer;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
};

#endif