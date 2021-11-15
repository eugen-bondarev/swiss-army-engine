#ifndef __D3D_SAMPLER_H__
#define __D3D_SAMPLER_H__

#pragma once

#include "dx_instance.h"

class Sampler
{
public:
    Sampler();
   ~Sampler() = default;

    void Bind();

private:
    ComPtr<ID3D11SamplerState> DXSampler;

    Sampler(const Sampler&) = delete;
    Sampler& operator=(const Sampler&) = delete;
};

#endif