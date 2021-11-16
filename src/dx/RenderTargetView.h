#ifndef __DX_RenderTargetView_h__
#define __DX_RenderTargetView_h__

#pragma once

#include "instance.h"
#include "texture.h"

namespace DX {

class RenderTargetView
{
public:
    RenderTargetView(bool Depth = false);
   ~RenderTargetView() = default;

    Texture* GetTexture()
    {
        return DXTexture.get();
    }

    ID3D11RenderTargetView* GetDXRenderTarget()
    {
        return DXRenderTargetView.Get();
    }

    void Bind()
    {
        GetContext()->OMSetRenderTargets(1u, DXRenderTargetView.GetAddressOf(), GetDepthStencilView());

        if (DXDepthView)
        {
            GetContext()->ClearDepthStencilView(DXDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
        }
    }

protected:
    Ptr<Texture>                    DXTexture;
    ComPtr<ID3D11RenderTargetView>  DXRenderTargetView;
    ComPtr<ID3D11DepthStencilView>  DXDepthView;

    RenderTargetView(const RenderTargetView&) = delete;
    RenderTargetView& operator=(const RenderTargetView&) = delete;
};

}

#endif