#ifndef __DX_D3D_INSTANCE_H__
#define __DX_D3D_INSTANCE_H__

#pragma once

#include "SwapChain.h"
#include "Debugger.h"
#include "Common.h"

class Window;

namespace DX {

class RenderTargetView;

Debugger*                           GetDebugger();
ID3D11Device*                       GetDevice();
ID3D11DeviceContext*                GetContext();
SwapChain*                          GetSwapChain();
Ref<RenderTargetView>&              GetRenderTargetView();

class Instance final
{

friend class Window;

friend Debugger*                    GetDebugger();
friend ID3D11Device*                GetDevice();
friend ID3D11DeviceContext*         GetContext();
friend SwapChain*                   GetSwapChain();
friend Ref<RenderTargetView>&       GetRenderTargetView();

public:
    Instance(Window* Wnd);
   ~Instance() = default;

    static void SetViewport(const UINT X, const UINT Y, const UINT Width, const UINT Height);

private:
    ComPtr<ID3D11Device>            DXDevice;
    ComPtr<ID3D11DeviceContext>     DXContext;
    Ptr<SwapChain>                  DXSwapChain;
    Ref<RenderTargetView>           DXRenderTargetView;

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