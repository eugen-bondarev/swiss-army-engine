#ifndef __DX_Sampler_h__
#define __DX_Sampler_h__

#pragma once

#include "Instance.h"

namespace DX
{
    class Sampler
    {
    public:
        Sampler();
       ~Sampler() = default;

        void Bind();

    private:
        ComPtr<ID3D11SamplerState> dxSampler;

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };
}

#endif