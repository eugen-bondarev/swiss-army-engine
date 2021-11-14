#ifndef __D3D_BUFFER_H__
#define __D3D_BUFFER_H__

#pragma once

#include "common.h"

class Buffer
{
public:
    Buffer(const UINT byteWidth, const UINT stride, const void* data, const UINT bindFlags, const UINT cpuAccessFlags = 0u, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
   ~Buffer() = default;

    void* Map();
    void  Unmap();

protected:
    ComPtr<ID3D11Buffer> dxBuffer;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
};

class VertexBuffer : public Buffer
{
public:
    VertexBuffer(const UINT byteWidth, const UINT stride = 0u, const void* data = nullptr);
    void Bind(const UINT stride, const UINT offset);
};

class IndexBuffer : public Buffer
{
public:
    IndexBuffer(const UINT byteWidth, const UINT stride = 0u, const void* data = nullptr);
    void Bind();
};

class ConstantBuffer : public Buffer
{
public:
    ConstantBuffer(const UINT byteWidth, const UINT stride = 0u, const void* data = nullptr);
    void Bind();
};

#endif