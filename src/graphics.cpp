#include "graphics.h"
#include "d3d/common.h"

std::unique_ptr<Debugger> Graphics::debugger;

Graphics::Graphics(HWND handle)
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

    #ifndef NDEBUG
        #define DEBUG_LAYER D3D11_CREATE_DEVICE_DEBUG
    #else
        #define DEBUG_LAYER 0
    #endif

    D3D_CHECK(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        DEBUG_LAYER,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swapchain,
        &device,
        nullptr,
        &context
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

    context->OMSetDepthStencilState(depthStencilState.Get(), 1u);

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

    context->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), depthView.Get());

    debugger = std::make_unique<Debugger>();

    common::device = device.Get();
    common::ctx = context.Get();
}