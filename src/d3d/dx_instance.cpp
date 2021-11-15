#include "dx_instance.h"

#include <map>

static std::map<HWND, D3D11Instance*> windows;
static D3D11Instance* currentCtx{nullptr};

D3D11Instance::D3D11Instance(HWND handle)
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
    swapChainDesc.OutputWindow = handle;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    const UINT flags = 
#ifndef NDEBUG
        D3D11_CREATE_DEVICE_DEBUG;
#else
        0;
#endif

    __D3D_TRY(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &DXSwapChain,
        &DXDevice,
        nullptr,
        &DXContext
    ));

#ifndef NDEBUG
    DXDebugger = new ID3D11Debugger();
#endif

    windows[handle] = this;
    currentCtx = this;

    ComPtr<ID3D11Resource> backBuffer{nullptr};
    D3D_TRY(DXSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
    D3D_TRY(DXDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &DXRenderTargetView));

    D3D11_DEPTH_STENCIL_DESC depthBufferDesc{};
    depthBufferDesc.DepthEnable = TRUE;
    depthBufferDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthBufferDesc.DepthFunc = D3D11_COMPARISON_LESS;
    ComPtr<ID3D11DepthStencilState> depthStencilState;
    D3D_TRY(DXDevice->CreateDepthStencilState(&depthBufferDesc, &depthStencilState));

    DXContext->OMSetDepthStencilState(depthStencilState.Get(), 1u);

    ComPtr<ID3D11Texture2D> depthTexture;
    D3D11_TEXTURE2D_DESC depthTextureDesc{};
    depthTextureDesc.Width = 800u;
    depthTextureDesc.Height = 600u;
    depthTextureDesc.MipLevels = 1u;
    depthTextureDesc.ArraySize = 1u;
    depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthTextureDesc.SampleDesc.Count = 1u;
    depthTextureDesc.SampleDesc.Quality = 0u;
    depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    D3D_TRY(DXDevice->CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture));

    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc{};
    depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthViewDesc.Texture2D.MipSlice = 0u;
    D3D_TRY(DXDevice->CreateDepthStencilView(depthTexture.Get(), &depthViewDesc, &DXDepthView));

    DXContext->OMSetRenderTargets(1u, DXRenderTargetView.GetAddressOf(), DXDepthView.Get());
}


void D3D11Instance::SetViewport(const UINT X, const UINT Y, const UINT Width, const UINT Height)
{
    D3D11_VIEWPORT viewport;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = X;
    viewport.TopLeftY = Y;
    Ctx()->RSSetViewports(1u, &viewport);
}

void MakeContextCurrent(D3D11Instance* newCtx)
{
    currentCtx = newCtx;
}

D3D11Instance* GetContext(HWND handle)
{
    return windows[handle];
}

#ifndef NDEBUG
ID3D11Debugger* Debugger()
{
    return currentCtx->DXDebugger;
}
#endif

ID3D11Device* Device()
{
    return currentCtx->DXDevice.Get();
}

ID3D11DeviceContext* Ctx()
{
    return currentCtx->DXContext.Get();
}

IDXGISwapChain* Swapchain()
{
    return currentCtx->DXSwapChain.Get();
}

ID3D11RenderTargetView* RenderTargetView()
{
    return currentCtx->DXRenderTargetView.Get();
}

ID3D11DepthStencilView* DepthStencilView()
{
    return currentCtx->DXDepthView.Get();
}

D3D11Instance::~D3D11Instance()
{
#ifndef NDEBUG
    delete DXDebugger;
#endif
}