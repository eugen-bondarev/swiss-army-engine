#include "Buffer.h"

#include "Device/Device.h"

namespace DX
{
    Buffer::Buffer(const UINT byteWidth, const UINT stride, const void *data, const UINT bindFlags, const UINT cpuAccessFlags, const D3D11_USAGE usage, Device& device) : stride{stride}, device{device}
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
            DX_TRY(device.GetDxDevice().CreateBuffer(&bufferDesc, &bufferSubData, &dxBuffer));
        }
        else
        {
            DX_TRY(device.GetDxDevice().CreateBuffer(&bufferDesc, nullptr, &dxBuffer));
        }
    }

    void *Buffer::Map()
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        device.GetDxContext().Map(dxBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource);
        return mappedResource.pData;
    }

    void Buffer::Unmap()
    {
        device.GetDxContext().Unmap(dxBuffer.Get(), 0u);
    }

    VertexBuffer::VertexBuffer(const UINT byteWidth, const UINT stride, const void *data, Device& device) : ::DX::Buffer(byteWidth, stride, data, D3D11_BIND_VERTEX_BUFFER, 0u, D3D11_USAGE_DEFAULT, device)
    {
    }

    IndexBuffer::IndexBuffer(const UINT ByteWidth, const UINT stride, const void *data, Device& device) : ::DX::Buffer(ByteWidth, stride, data, D3D11_BIND_INDEX_BUFFER, 0u, D3D11_USAGE_DEFAULT, device)
    {
    }

    ConstantBuffer::ConstantBuffer(const UINT ByteWidth, const UINT stride, const void *data, Device& device) : ::DX::Buffer(ByteWidth, stride, data, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC, device)
    {
    }

    void VertexBuffer::Bind(const UINT offset)
    {
        device.GetDxContext().IASetVertexBuffers(0u, 1u, dxBuffer.GetAddressOf(), &stride, &offset);
    }

    void IndexBuffer::Bind()
    {
        device.GetDxContext().IASetIndexBuffer(dxBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
    }

    void ConstantBuffer::Bind()
    {
        device.GetDxContext().VSSetConstantBuffers(0u, 1u, dxBuffer.GetAddressOf());
    }

    void *VertexBuffer::Map()
    {
        return ::DX::Buffer::Map();
    }

    void VertexBuffer::Unmap()
    {
        ::DX::Buffer::Unmap();
    }

    void *IndexBuffer::Map()
    {
        return ::DX::Buffer::Map();
    }

    void IndexBuffer::Unmap()
    {
        ::DX::Buffer::Unmap();
    }

    void *ConstantBuffer::Map()
    {
        return ::DX::Buffer::Map();
    }

    void ConstantBuffer::Unmap()
    {
        ::DX::Buffer::Unmap();
    }
}