#ifndef __D3D_SAMPLER_H__
#define __D3D_SAMPLER_H__

#pragma once

#include "common.h"
#include "d3d.h"

class Sampler
{
public:
    Sampler()
    {
        D3D11_SAMPLER_DESC samplerDesc{};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        D3D_TRY(Device()->CreateSamplerState(&samplerDesc, &sampler));
    }

   ~Sampler() = default;

    void Bind()
    {
        Ctx()->PSSetSamplers(0u, 1u, sampler.GetAddressOf());
    }

private:
    ComPtr<ID3D11SamplerState> sampler;

    Sampler(const Sampler&) = delete;
    Sampler& operator=(const Sampler&) = delete;
};

#endif