#ifndef __DX_DepthBuffer_h__
#define __DX_DepthBuffer_h__

#pragma once

#include "Instance.h"

namespace DX {

class DepthBuffer
{
public:
    DepthBuffer(const unsigned int Width, const unsigned int Height)
    {
        ComPtr<ID3D11Texture2D> depthTexture;
        D3D11_TEXTURE2D_DESC depthTextureDesc{};
        depthTextureDesc.Width = Width;
        depthTextureDesc.Height = Height;
        depthTextureDesc.MipLevels = 1u;
        depthTextureDesc.ArraySize = 1u;
        depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthTextureDesc.SampleDesc.Count = 1u;
        depthTextureDesc.SampleDesc.Quality = 0u;
        depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        D3D_TRY(GetDevice()->CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture));

        D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc{};
        depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthViewDesc.Texture2D.MipSlice = 0u;
        D3D_TRY(GetDevice()->CreateDepthStencilView(depthTexture.Get(), &depthViewDesc, &DXDepthView));
    }

   ~DepthBuffer() = default;

    void Clear()
    {
        GetContext()->ClearDepthStencilView(DXDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    }

    ID3D11DepthStencilView* GetDXDepthStencilView()
    {
        return DXDepthView.Get();
    }

private:
    ComPtr<ID3D11DepthStencilView>  DXDepthView;
    
    DepthBuffer(const DepthBuffer&) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;
};

}

#endif