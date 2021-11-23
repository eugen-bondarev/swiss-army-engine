#include "Texture.h"

#include "GraphicsContext.h"

#include "../DX/Texture.h"

namespace API
{
    Ptr<Texture> Texture::Create(const unsigned int width, const unsigned int height, const unsigned char* data)
    {
        if (GetCurrentGraphicsContext()->GetAPIType() == API::Type::DirectX) return CreatePtr<DX::Texture>(width, height, data);
        return nullptr;
    }
}