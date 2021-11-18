#ifndef __DX_RenderTargetView_h__
#define __DX_RenderTargetView_h__

#pragma once

#include "DepthBuffer.h"
#include "SwapChain.h"
#include "Instance.h"
#include "Texture.h"

namespace DX
{
    class RenderTargetView
    {
    public:
        RenderTargetView(SwapChain* swapChain, const bool initDepth = false);
        RenderTargetView(const unsigned int width, const unsigned int height, const bool initDepth = false);
       ~RenderTargetView() = default;

        void Bind();
        void Unbind();
        void Clear(const std::array<float, 4>& clearColor = {0, 0, 0, 1});

        ID3D11RenderTargetView *GetDXRenderTarget();
        Texture *GetTexture();

    private:
        void Init(const unsigned int width, const unsigned int height, ID3D11Resource *resource, const bool initDepth);

    protected:
        ComPtr<ID3D11RenderTargetView> DXRenderTargetView;
        Ptr<DepthBuffer> DXDepthBuffer;
        Ptr<Texture> DXTexture;

        RenderTargetView(const RenderTargetView&) = delete;
        RenderTargetView &operator=(const RenderTargetView&) = delete;
    };
}

#endif