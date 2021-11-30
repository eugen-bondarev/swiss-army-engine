#include "Sampler.h"

#include "GraphicsContext.h"

#include "../DX/Sampler.h"

namespace API
{
    Ptr<Sampler> Sampler::Create()
    {
        if (GetCurrentGraphicsContext().GetAPIType() == API::Type::DirectX) return CreatePtr<DX::Sampler>();
        return nullptr;
    }
}