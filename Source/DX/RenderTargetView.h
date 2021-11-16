#ifndef __DX_RenderTargetView_h__
#define __DX_RenderTargetView_h__

#pragma once

#include "Instance.h"
#include "Texture.h"

namespace DX {

class RenderTargetView
{
public:
    RenderTargetView(ID3D11Resource* Resource = nullptr, bool Depth = false);
   ~RenderTargetView() = default;

    void Bind();
    void Clear(const std::array<float, 4>& ClearColor = {0, 0, 0, 1});

    ID3D11RenderTargetView* GetDXRenderTarget();
    Texture* GetTexture();

protected:
    ComPtr<ID3D11RenderTargetView>  DXRenderTargetView;
    ComPtr<ID3D11DepthStencilView>  DXDepthView;
    Ptr<Texture>                    DXTexture;

    RenderTargetView(const RenderTargetView&) = delete;
    RenderTargetView& operator=(const RenderTargetView&) = delete;
};

}

#endif