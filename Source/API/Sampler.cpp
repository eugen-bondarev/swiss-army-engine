#include "Sampler.h"

#include "Instance.h"

#include "../DX/Sampler.h"

namespace API
{
    Ptr<Sampler> Sampler::Create()
    {
        if (GetCurrentInstance()->GetAPIType() == API::Type::DirectX) return CreatePtr<DX::Sampler>();
        return nullptr;
    }
}