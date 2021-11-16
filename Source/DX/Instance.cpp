#include "Instance.h"

#include "RenderTargetView.h"
#include "../Window/Window.h"

namespace DX {

static std::map<Window*, Instance*>   Instances;
static Instance*                   CurrentInstance{nullptr};

Instance::Instance(Window* Wnd)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = glfwGetWin32Window(Wnd->GetHandle());
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    DXSwapChain = CreatePtr<SwapChain>();
    DXSwapChain->Width = 1920;
    DXSwapChain->Height = 1080;

    Wnd->SwapChain = DXSwapChain.get();

    const UINT flags = 
#ifndef NDEBUG
        D3D11_CREATE_DEVICE_DEBUG;
#else
        0;
#endif

    __DX_TRY(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &DXSwapChain->DXSwapChain,
        &DXDevice,
        nullptr,
        &DXContext
    ));

#ifndef NDEBUG
    DXDebugger = CreatePtr<Debugger>();
#endif

    Instances[Wnd] = this;
    CurrentInstance = this;

    DXRenderTargetView = CreateRef<RenderTargetView>(DXSwapChain.get(), false);
    DXRenderTargetView->Bind();

    D3D11_DEPTH_STENCIL_DESC depthBufferDesc{};
    depthBufferDesc.DepthEnable = TRUE;
    depthBufferDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthBufferDesc.DepthFunc = D3D11_COMPARISON_LESS;
    ComPtr<ID3D11DepthStencilState> depthStencilState;
    D3D_TRY(DXDevice->CreateDepthStencilState(&depthBufferDesc, &depthStencilState));

    DXContext->OMSetDepthStencilState(depthStencilState.Get(), 1u);
}

void Instance::SetViewport(const UINT X, const UINT Y, const UINT Width, const UINT Height)
{
    D3D11_VIEWPORT viewport;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = X;
    viewport.TopLeftY = Y;
    GetContext()->RSSetViewports(1u, &viewport);
}

void MakeInstanceCurrent(Instance* NewContext)
{
    CurrentInstance = NewContext;
}

Instance* GetInstance(Window* Wnd)
{
    return Instances[Wnd];
}

#ifndef NDEBUG
Debugger* GetDebugger()
{
    return CurrentInstance->DXDebugger.get();
}
#endif

ID3D11Device* GetDevice()
{
    return CurrentInstance->DXDevice.Get();
}

ID3D11DeviceContext* GetContext()
{
    return CurrentInstance->DXContext.Get();
}

SwapChain* GetSwapChain()
{
    return CurrentInstance->DXSwapChain.get();
}

Ref<RenderTargetView>& GetRenderTargetView()
{
    return CurrentInstance->DXRenderTargetView;
}

}