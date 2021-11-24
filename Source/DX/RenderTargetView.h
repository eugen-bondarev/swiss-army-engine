#ifndef __DX_RenderTargetView_h__
#define __DX_RenderTargetView_h__

#pragma once

#include "GraphicsContext.h"
#include "DepthBuffer.h"
#include "SwapChain.h"
#include "Texture.h"

namespace DX
{
    class RenderTargetView
    {
    public:
        RenderTargetView(SwapChain* swapChain, const bool initDepth = false, Device& device = GetDevice());
        RenderTargetView(const Vec2ui size, const bool initDepth = false, Device& device = GetDevice());
       ~RenderTargetView() = default;

        void Bind();
        void Unbind();
        void Clear(const std::array<float, 4>& clearColor = {0, 0, 0, 1});

        ID3D11RenderTargetView* GetDXRenderTarget();
        Texture* GetTexture();

    private:
        void Init(const Vec2ui size, ID3D11Resource *resource, const bool initDepth);

    protected:
        Device& device;

        ComPtr<ID3D11RenderTargetView> dxRenderTargetView;
        Ptr<DepthBuffer> depthBuffer;
        Ptr<Texture> texture;

        RenderTargetView(const RenderTargetView&) = delete;
        RenderTargetView &operator=(const RenderTargetView&) = delete;
    };
}

#endif