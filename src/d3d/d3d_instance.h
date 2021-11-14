#ifndef __D3D_D3D_INSTANCE_H__
#define __D3D_D3D_INSTANCE_H__

#pragma once

#include <d3d11.h>

#include <wrl.h>

#include "common.h"
#include "debugger.h"

// Forward declaration
class D3DInstance;

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

// Function declarations
#ifndef NDEBUG
Debugger* GetDebugger();
#endif

ID3D11Device* Device();
ID3D11DeviceContext* Ctx();
IDXGISwapChain* Swapchain();
ID3D11RenderTargetView* RenderTargetView();
ID3D11DepthStencilView* DepthStencilView();
D3DInstance* GetContext(HWND handle);
void MakeContextCurrent(D3DInstance* newCtx);

class D3DInstance
{
friend class Window;

friend Debugger* GetDebugger();
friend ID3D11Device* Device();
friend ID3D11DeviceContext* Ctx();
friend IDXGISwapChain* Swapchain();
friend ID3D11RenderTargetView* RenderTargetView();
friend ID3D11DepthStencilView* DepthStencilView();

public:
    D3DInstance(HWND handle);
   ~D3DInstance();

   static void SetViewport(const UINT X, const UINT Y, const UINT Width, const UINT Height);

private:
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> ctx;
    ComPtr<IDXGISwapChain> swapchain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    ComPtr<ID3D11DepthStencilView> depthView;

    #ifndef NDEBUG
    Debugger* debugger;
    #endif

private:
    D3DInstance(const D3DInstance&) = delete;
    D3DInstance& operator=(const D3DInstance&) = delete;
};

#ifndef NDEBUG
#   define D3D_TRY(exp)\
        GetDebugger()->Start();\
        exp;\
        GetDebugger()->End(__FILE__, __LINE__)
#else
#   define D3D_TRY(exp)\
        exp
#endif

#endif