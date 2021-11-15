#include "buffer.h"

Buffer::Buffer(const UINT byteWidth, const UINT stride, const void* data, const UINT bindFlags, const UINT cpuAccessFlags, D3D11_USAGE usage)
{    
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.BindFlags = bindFlags;
    bufferDesc.Usage = usage;
    bufferDesc.CPUAccessFlags = cpuAccessFlags;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.ByteWidth = byteWidth;
    bufferDesc.StructureByteStride = stride;

    if (data)
    {
        D3D11_SUBRESOURCE_DATA bufferSubData{};
        bufferSubData.pSysMem = data;
        D3D_TRY(Device()->CreateBuffer(&bufferDesc, &bufferSubData, &dxBuffer));
    }
    else
    {
        D3D_TRY(Device()->CreateBuffer(&bufferDesc, nullptr, &dxBuffer));
    }
}

void* Buffer::Map()
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    Ctx()->Map(dxBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource);
    return mappedResource.pData;
}

void Buffer::Unmap()
{
    Ctx()->Unmap(dxBuffer.Get(), 0u);
}

VertexBuffer::VertexBuffer(const UINT byteWidth, const UINT stride, const void* data) : Buffer(byteWidth, stride, data, D3D11_BIND_VERTEX_BUFFER)
{
}

IndexBuffer::IndexBuffer(const UINT byteWidth, const UINT stride, const void* data) : Buffer(byteWidth, stride, data, D3D11_BIND_INDEX_BUFFER)
{
}

ConstantBuffer::ConstantBuffer(const UINT byteWidth, const UINT stride, const void* data) : Buffer(byteWidth, stride, data, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC)
{
}

void VertexBuffer::Bind(const UINT stride, const UINT offset)
{
    Ctx()->IASetVertexBuffers(0u, 1u, dxBuffer.GetAddressOf(), &stride, &offset);
}

void IndexBuffer::Bind()
{
    Ctx()->IASetIndexBuffer(dxBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

void ConstantBuffer::Bind()
{
    Ctx()->VSSetConstantBuffers(0u, 1u, dxBuffer.GetAddressOf());
}