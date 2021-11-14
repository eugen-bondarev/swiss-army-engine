#ifndef __D3D_D3D_H__
#define __D3D_D3D_H__

#pragma once

#include <d3d11.h>

#include <wrl.h>

#include "common.h"
#include "debugger.h"

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class D3D
{
friend class Window;

friend Debugger* GetDebugger();
friend ID3D11Device* Device();
friend ID3D11DeviceContext* Ctx();
friend IDXGISwapChain* Swapchain();
friend ID3D11RenderTargetView* RenderTargetView();
friend ID3D11DepthStencilView* DepthStencilView();

public:
    D3D(HWND handle);
   ~D3D();

private:
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> ctx;
    ComPtr<IDXGISwapChain> swapchain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    ComPtr<ID3D11DepthStencilView> depthView;
    Debugger* debugger;

private:
    D3D(const D3D&) = delete;
    D3D& operator=(const D3D&) = delete;
};

Debugger* GetDebugger();
ID3D11Device* Device();
ID3D11DeviceContext* Ctx();
IDXGISwapChain* Swapchain();
ID3D11RenderTargetView* RenderTargetView();
ID3D11DepthStencilView* DepthStencilView();

void MakeContextCurrent(D3D* another);
D3D* GetContext(HWND handle);

#define D3D_TRY(exp)\
    GetDebugger()->Start();\
    exp;\
    GetDebugger()->End(__FILE__, __LINE__)

#endif