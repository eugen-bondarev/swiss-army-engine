#include "sampler.h"

Sampler::Sampler()
{
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    D3D_TRY(Device()->CreateSamplerState(&samplerDesc, &DXSampler));
}

void Sampler::Bind()
{
    Ctx()->PSSetSamplers(0u, 1u, DXSampler.GetAddressOf());
}