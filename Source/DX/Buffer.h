#ifndef __DX_Buffer_h__
#define __DX_Buffer_h__

#pragma once

#include "Instance.h"

namespace DX {

class Buffer
{
public:
    Buffer(const UINT ByteWidth, const UINT Stride, const void* Data, const UINT BindFlags, const UINT CPUAccessFlags = 0u, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT);
   ~Buffer() = default;

    void* Map();
    void Unmap();

protected:
    ComPtr<ID3D11Buffer> DXBuffer;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
};

class VertexBuffer : public Buffer
{
public:
    VertexBuffer(const UINT ByteWidth, const UINT Stride = 0u, const void* Data = nullptr);
    void Bind(const UINT Stride, const UINT Offset);
};

class IndexBuffer : public Buffer
{
public:
    IndexBuffer(const UINT ByteWidth, const UINT Stride = 0u, const void* Data = nullptr);
    void Bind();
};

class ConstantBuffer : public Buffer
{
public:
    ConstantBuffer(const UINT ByteWidth, const UINT Stride = 0u, const void* Data = nullptr);
    void Bind();
};

}

#endif