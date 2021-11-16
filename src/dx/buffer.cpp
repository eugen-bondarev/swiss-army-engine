#include "Buffer.h"

namespace DX {

Buffer::Buffer(const UINT ByteWidth, const UINT Stride, const void* Data, const UINT BindFlags, const UINT CPUAccessFlags, D3D11_USAGE Usage)
{    
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.BindFlags = BindFlags;
    bufferDesc.Usage = Usage;
    bufferDesc.CPUAccessFlags = CPUAccessFlags;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.ByteWidth = ByteWidth;
    bufferDesc.StructureByteStride = Stride;

    if (Data)
    {
        D3D11_SUBRESOURCE_DATA bufferSubData{};
        bufferSubData.pSysMem = Data;
        D3D_TRY(GetDevice()->CreateBuffer(&bufferDesc, &bufferSubData, &DXBuffer));
    }
    else
    {
        D3D_TRY(GetDevice()->CreateBuffer(&bufferDesc, nullptr, &DXBuffer));
    }
}

void* Buffer::Map()
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    GetContext()->Map(DXBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource);
    return mappedResource.pData;
}

void Buffer::Unmap()
{
    GetContext()->Unmap(DXBuffer.Get(), 0u);
}

VertexBuffer::VertexBuffer(const UINT ByteWidth, const UINT Stride, const void* Data) : Buffer(ByteWidth, Stride, Data, D3D11_BIND_VERTEX_BUFFER)
{
}

IndexBuffer::IndexBuffer(const UINT ByteWidth, const UINT Stride, const void* Data) : Buffer(ByteWidth, Stride, Data, D3D11_BIND_INDEX_BUFFER)
{
}

ConstantBuffer::ConstantBuffer(const UINT ByteWidth, const UINT Stride, const void* Data) : Buffer(ByteWidth, Stride, Data, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC)
{
}

void VertexBuffer::Bind(const UINT Stride, const UINT Offset)
{
    GetContext()->IASetVertexBuffers(0u, 1u, DXBuffer.GetAddressOf(), &Stride, &Offset);
}

void IndexBuffer::Bind()
{
    GetContext()->IASetIndexBuffer(DXBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

void ConstantBuffer::Bind()
{
    GetContext()->VSSetConstantBuffers(0u, 1u, DXBuffer.GetAddressOf());
}

}