#include "graphics.h"

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

    debugger = std::make_unique<Debugger>();
}