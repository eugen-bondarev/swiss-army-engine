#ifndef __DX_Instance_h__
#define __DX_Instance_h__

#pragma once

#include "../API/Instance.h"

#include "SwapChain.h"
#include "Debugger.h"
#include "Common.h"

FORWARD_DECLARE(Window);

namespace DX
{
    FORWARD_DECLARE(RenderTargetView);

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetContext();
    Debugger* GetDebugger();
    SwapChain* GetSwapChain();
    Ref<RenderTargetView>& GetRenderTargetView();

    class Instance final : public API::Instance
    {
    friend class Window;

    friend ID3D11Device* GetDevice();
    friend ID3D11DeviceContext* GetContext();
    friend Debugger* GetDebugger();
    friend SwapChain* GetSwapChain();
    friend Ref<RenderTargetView>& GetRenderTargetView();

    public:
        Instance(Window& window);
       ~Instance() = default;

        void SetViewport(const UINT width, const UINT height, const UINT x = 0u, const UINT y = 0u);
        void OnResize(const unsigned int width, const unsigned int height);

        API::Type GetAPIType() const override;

    private:
        ComPtr<ID3D11Device>        dxDevice;
        ComPtr<ID3D11DeviceContext> dxContext;
        Ptr<SwapChain>              swapChain;
        Ref<RenderTargetView>       renderTargetView;

#ifndef NDEBUG
        Ptr<Debugger>               debugger;
#endif

        Instance(const Instance&) = delete;
        Instance& operator=(const Instance&) = delete;
    };
}

#endif