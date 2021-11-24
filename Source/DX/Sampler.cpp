#include "Sampler.h"

#include "Device/Device.h"

namespace DX
{
    Sampler::Sampler(Device& device) : device{device}
    {
        D3D11_SAMPLER_DESC samplerDesc{};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        DX_TRY(device.GetDxDevice().CreateSamplerState(&samplerDesc, &dxSampler));
    }

    void Sampler::Bind()
    {
        device.GetDxContext().PSSetSamplers(0u, 1u, dxSampler.GetAddressOf());
    }
}