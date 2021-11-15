#ifndef __D3D_D3D_INSTANCE_H__
#define __D3D_D3D_INSTANCE_H__

#pragma once

#include "common.h"
#include "dx_debugger.h"

class D3D11Instance final
{

friend class Window;

friend ID3D11Debugger* Debugger();
friend ID3D11Device* Device();
friend ID3D11DeviceContext* Ctx();
friend IDXGISwapChain* Swapchain();
friend ID3D11RenderTargetView* RenderTargetView();
friend ID3D11DepthStencilView* DepthStencilView();

public:
    D3D11Instance(HWND Handle);
   ~D3D11Instance();

    static void SetViewport(const UINT X, const UINT Y, const UINT Width, const UINT Height);

private:
    ComPtr<ID3D11Device> DXDevice;
    ComPtr<ID3D11DeviceContext> DXContext;
    ComPtr<IDXGISwapChain> DXSwapChain;
    ComPtr<ID3D11RenderTargetView> DXRenderTargetView;
    ComPtr<ID3D11DepthStencilView> DXDepthView;
    #ifndef NDEBUG
    ID3D11Debugger* DXDebugger;
    #endif

private:
    D3D11Instance(const D3D11Instance&) = delete;
    D3D11Instance& operator=(const D3D11Instance&) = delete;
};

#endif