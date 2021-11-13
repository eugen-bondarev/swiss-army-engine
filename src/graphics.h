#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#pragma once

#include "debugger.h"
#include "header.h"

class Graphics
{
public:
    Graphics(HWND handle);

    ComPtr<ID3D11Device> device{nullptr};
    ComPtr<IDXGISwapChain> swapchain{nullptr};
    ComPtr<ID3D11DeviceContext> context{nullptr};
    ComPtr<ID3D11RenderTargetView> renderTargetView;

    ComPtr<ID3D11DepthStencilView> depthView;

    static std::unique_ptr<Debugger> debugger;
};

#define D3D_TRY(exp)\
    ::Graphics::debugger->Start();\
    exp;\
    ::Graphics::debugger->End(__FILE__, __LINE__)

#endif