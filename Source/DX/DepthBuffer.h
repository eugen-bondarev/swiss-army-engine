#ifndef __DX_DepthBuffer_h__
#define __DX_DepthBuffer_h__

#pragma once

#include "GraphicsContext.h"

namespace DX 
{
    class DepthBuffer
    {
    public:
        DepthBuffer(const Vec2ui size, Device& device = GetDevice());
       ~DepthBuffer() = default;

        void Clear();

        ID3D11DepthStencilView* GetDXDepthStencilView();

    private:
        Device& device;

        ComPtr<ID3D11DepthStencilView> dxDepthView;
        
        DepthBuffer(const DepthBuffer&) = delete;
        DepthBuffer& operator=(const DepthBuffer&) = delete;
    };
}

#endif