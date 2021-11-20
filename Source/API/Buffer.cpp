#include "Buffer.h"

#include "../DX/Buffer.h"

namespace API
{
    Ptr<VertexBuffer> VertexBuffer::Create(const unsigned int byteWidth, const unsigned int stride, const void* data)
    {
        if (GetCurrentInstance()->GetAPIType() == API::Type::DirectX) return CreatePtr<DX::VertexBuffer>(byteWidth, stride, data);
        return nullptr;
    }

    Ptr<IndexBuffer> IndexBuffer::Create(const unsigned int byteWidth, const unsigned int stride, const void* data)
    {
        if (GetCurrentInstance()->GetAPIType() == API::Type::DirectX) return CreatePtr<DX::IndexBuffer>(byteWidth, stride, data);
        return nullptr;
    }

    Ptr<UniformBuffer> UniformBuffer::Create(const unsigned int byteWidth, const unsigned int stride, const void* data)
    {
        if (GetCurrentInstance()->GetAPIType() == API::Type::DirectX) return CreatePtr<DX::ConstantBuffer>(byteWidth, stride, data);
        return nullptr;
    }
}