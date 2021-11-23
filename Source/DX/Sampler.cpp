#include "Sampler.h"

namespace DX
{
    Sampler::Sampler()
    {
        D3D11_SAMPLER_DESC samplerDesc{};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        D3D_TRY(GetDevice()->CreateSamplerState(&samplerDesc, &dxSampler));
    }

    void Sampler::Bind()
    {
        GetContext()->PSSetSamplers(0u, 1u, dxSampler.GetAddressOf());
    }
}