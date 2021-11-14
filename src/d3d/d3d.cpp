#include "d3d.h"

#include <map>

static std::map<HWND, D3D*> windows;
static D3D* currentCtx{nullptr};

D3D::D3D(HWND handle)
{
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = handle;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    const UINT flags = 
#ifndef NDEBUG
        D3D11_CREATE_DEVICE_DEBUG;
#else
        0;
#endif

    D3D_CHECK(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swapchain,
        &device,
        nullptr,
        &ctx
    ));

    ComPtr<ID3D11Resource> backBuffer{nullptr};
    D3D_CHECK(swapchain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
    D3D_CHECK(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

    D3D11_DEPTH_STENCIL_DESC depthBufferDesc{};
    depthBufferDesc.DepthEnable = TRUE;
    depthBufferDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthBufferDesc.DepthFunc = D3D11_COMPARISON_LESS;
    ComPtr<ID3D11DepthStencilState> depthStencilState;
    D3D_CHECK(device->CreateDepthStencilState(&depthBufferDesc, &depthStencilState));

    ctx->OMSetDepthStencilState(depthStencilState.Get(), 1u);

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
    D3D_CHECK(device->CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture));

    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc{};
    depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthViewDesc.Texture2D.MipSlice = 0u;
    D3D_CHECK(device->CreateDepthStencilView(depthTexture.Get(), &depthViewDesc, &depthView));

    ctx->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), depthView.Get());

#ifndef NDEBUG
    debugger = new Debugger();
#endif

    windows[handle] = this;
    currentCtx = this;
}

void MakeContextCurrent(D3D* newCtx)
{
    currentCtx = newCtx;
}

D3D* GetContext(HWND handle)
{
    return windows[handle];
}

#ifndef NDEBUG
Debugger* GetDebugger()
{
    return currentCtx->debugger;
}
#endif

ID3D11Device* Device()
{
    return currentCtx->device.Get();
}

ID3D11DeviceContext* Ctx()
{
    return currentCtx->ctx.Get();
}

IDXGISwapChain* Swapchain()
{
    return currentCtx->swapchain.Get();
}

ID3D11RenderTargetView* RenderTargetView()
{
    return currentCtx->renderTargetView.Get();
}

ID3D11DepthStencilView* DepthStencilView()
{
    return currentCtx->depthView.Get();
}

D3D::~D3D()
{
#ifndef NDEBUG
    delete debugger;
#endif
}