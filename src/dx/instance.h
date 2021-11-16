#ifndef __DX_D3D_INSTANCE_H__
#define __DX_D3D_INSTANCE_H__

#pragma once

#include "Debugger.h"
#include "Common.h"

namespace DX {

class RenderTargetView;

Debugger*                           GetDebugger();
ID3D11Device*                       GetDevice();
ID3D11DeviceContext*                GetContext();
IDXGISwapChain*                     GetSwapChain();
RenderTargetView*                   GetRenderTargetView();

class Instance final
{

friend class Window;

friend Debugger*                    GetDebugger();
friend ID3D11Device*                GetDevice();
friend ID3D11DeviceContext*         GetContext();
friend IDXGISwapChain*              GetSwapChain();
friend RenderTargetView*            GetRenderTargetView();

public:
    Instance(HWND Handle);
   ~Instance() = default;

    static void SetViewport(const UINT X, const UINT Y, const UINT Width, const UINT Height);

private:
    ComPtr<ID3D11Device>            DXDevice;
    ComPtr<ID3D11DeviceContext>     DXContext;
    ComPtr<IDXGISwapChain>          DXSwapChain;
    Ptr<RenderTargetView>           DXRenderTargetView;

    #ifndef NDEBUG
    Ptr<Debugger> DXDebugger;
    #endif

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;
};

Instance* GetInstance(HWND Handle);
void MakeInstanceCurrent(Instance* NewContext);

}

#endif