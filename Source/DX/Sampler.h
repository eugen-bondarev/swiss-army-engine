#ifndef __DX_Sampler_h__
#define __DX_Sampler_h__

#pragma once

#include "GraphicsContext.h"
#include "../API/Sampler.h"

namespace DX
{
    class Sampler : public API::Sampler
    {
    public:
        Sampler(Device& device = GetDevice());
       ~Sampler() = default;

        void Bind() override;

    private:
        Device& device;

        ComPtr<ID3D11SamplerState> dxSampler;

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };
}

#endif