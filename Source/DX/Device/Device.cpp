#include "Device.h"

namespace DX
{
    ID3D11Device& Device::GetDxDevice()
    {
        return *dxDevice.Get();
    }

    ID3D11DeviceContext& Device::GetDxContext()
    {
        return *dxContext.Get();
    }
}