#ifndef __DX_Device_Device_h__
#define __DX_Device_Device_h__

#pragma once

#include "../Common.h"

namespace DX
{
    class Device
    {
    friend class GraphicsContext;

    public:
        Device() = default;
       ~Device() = default;

        ID3D11Device& GetDxDevice();
        ID3D11DeviceContext& GetDxContext();

    private:
        ComPtr<ID3D11Device> dxDevice;
        ComPtr<ID3D11DeviceContext> dxContext;

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;
    };
}

#endif