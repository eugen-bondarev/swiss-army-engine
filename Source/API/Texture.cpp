#include "Texture.h"

#include "GraphicsContext.h"

#include "../DX/Texture.h"

namespace API
{
    Ptr<Texture> Texture::Create(const Vec2ui size, const unsigned char* data)
    {
        if (GetCurrentGraphicsContext()->GetAPIType() == API::Type::DirectX) return CreatePtr<DX::Texture>(size, data);
        return nullptr;
    }
}