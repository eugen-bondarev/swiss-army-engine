#include "Buffer.h"

#include "../DX/Buffer.h"
// #include "../VK/Entities/Buffer/Buffer.h"

namespace API
{
    Ptr<VertexBuffer> VertexBuffer::Create(const unsigned int byteWidth, const unsigned int stride, const void* data)
    {
        if (GetCurrentGraphicsContext().GetAPIType() == API::Type::DirectX) return CreatePtr<DX::VertexBuffer>(byteWidth, stride, data);
        // if (GetCurrentGraphicsContext()->GetAPIType() == API::Type::Vulkan) return CreatePtr<VK::VertexBuffer>(byteWidth, stride, data);
        return nullptr;
    }

    Ptr<IndexBuffer> IndexBuffer::Create(const unsigned int byteWidth, const unsigned int stride, const void* data)
    {
        if (GetCurrentGraphicsContext().GetAPIType() == API::Type::DirectX) return CreatePtr<DX::IndexBuffer>(byteWidth, stride, data);
        // if (GetCurrentGraphicsContext()->GetAPIType() == API::Type::Vulkan) return CreatePtr<VK::IndexBuffer>(byteWidth, stride, data);
        return nullptr;
    }

    Ptr<UniformBuffer> UniformBuffer::Create(const unsigned int byteWidth, const unsigned int stride, const void* data)
    {
        if (GetCurrentGraphicsContext().GetAPIType() == API::Type::DirectX) return CreatePtr<DX::UpdatableConstantBuffer>(byteWidth, stride, data);
        return nullptr;
    }
}