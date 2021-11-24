#ifndef __DX_GraphicsContext_h__
#define __DX_GraphicsContext_h__

#pragma once

#include "../API/GraphicsContext.h"
#include "SwapChain.h"
#include "Debugger.h"
#include "Common.h"

FORWARD_DECLARE(RawWindow);

namespace DX
{
    FORWARD_DECLARE(RenderTargetView);

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetContext();
    Debugger* GetDebugger();
    SwapChain* GetSwapChain();
    Ref<RenderTargetView>& GetRenderTargetView();

    class GraphicsContext final : public API::GraphicsContext
    {
    friend class RawWindow;

    friend ID3D11Device* GetDevice();
    friend ID3D11DeviceContext* GetContext();
    friend Debugger* GetDebugger();
    friend SwapChain* GetSwapChain();
    friend Ref<RenderTargetView>& GetRenderTargetView();

    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext() = default;

        void SetViewport(const UINT width, const UINT height, const UINT x = 0u, const UINT y = 0u);
        void OnResize(const Vec2ui size);

        API::Type GetAPIType() const override;

    private:
        ComPtr<ID3D11Device>        dxDevice;
        ComPtr<ID3D11DeviceContext> dxContext;
        Ptr<SwapChain>              swapChain;
        Ref<RenderTargetView>       renderTargetView;

#ifndef NDEBUG
        Ptr<Debugger>               debugger;
#endif

        GraphicsContext(const GraphicsContext&) = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;
    };
}

#endif