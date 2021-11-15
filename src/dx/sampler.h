#ifndef __DX_SAMPLER_H__
#define __DX_SAMPLER_H__

#pragma once

#include "instance.h"

namespace DX {

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

}

#endif