#ifndef __DX_D3D_INSTANCE_H__
#define __DX_D3D_INSTANCE_H__

#pragma once

#include "debugger.h"
#include "common.h"

namespace DX {

Debugger*                           GetDebugger();
ID3D11Device*                       GetDevice();
ID3D11DeviceContext*                GetContext();
IDXGISwapChain*                     GetSwapChain();
ID3D11RenderTargetView*             GetRenderTargetView();
ID3D11DepthStencilView*             GetDepthStencilView();

class Instance final
{

friend class Window;

friend Debugger*                    GetDebugger();
friend ID3D11Device*                GetDevice();
friend ID3D11DeviceContext*         GetContext();
friend IDXGISwapChain*              GetSwapChain();
friend ID3D11RenderTargetView*      GetRenderTargetView();
friend ID3D11DepthStencilView*      GetDepthStencilView();

public:
    Instance(HWND Handle);
   ~Instance() = default;

    static void SetViewport(const UINT X, const UINT Y, const UINT Width, const UINT Height);

private:
    ComPtr<ID3D11Device>            DXDevice;
    ComPtr<ID3D11DeviceContext>     DXContext;
    ComPtr<IDXGISwapChain>          DXSwapChain;
    ComPtr<ID3D11RenderTargetView>  DXRenderTargetView;
    ComPtr<ID3D11DepthStencilView>  DXDepthView;

    #ifndef NDEBUG
    std::unique_ptr<Debugger> DXDebugger;
    #endif

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;
};

Instance* GetInstance(HWND Handle);
void MakeInstanceCurrent(Instance* NewContext);

}

#endif