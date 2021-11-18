#include "Instance.h"

#include "RenderTargetView.h"
#include "../Window/Window.h"

namespace DX
{
    static std::map<Window*, Instance*>     Instances;
    static Instance*                        CurrentInstance{nullptr};

    Instance::Instance(Window& window) : window{window}
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
        swapChainDesc.OutputWindow = glfwGetWin32Window(window.GetHandle());
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = 0;

        swapChain = CreatePtr<SwapChain>(window);        
        window.ResizeSubscribe(std::bind(&Instance::OnResize, this, std::placeholders::_1, std::placeholders::_2));

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
            &swapChain->DXSwapChain,
            &dxDevice,
            nullptr,
            &dxContext));

#ifndef NDEBUG
        debugger = CreatePtr<Debugger>();
#endif

        Instances[&window] = this;
        CurrentInstance = this;

        renderTargetView = CreateRef<RenderTargetView>(swapChain.get(), true);
        renderTargetView->Bind();

        D3D11_DEPTH_STENCIL_DESC depthBufferDesc{};
        depthBufferDesc.DepthEnable = TRUE;
        depthBufferDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthBufferDesc.DepthFunc = D3D11_COMPARISON_LESS;
        ComPtr<ID3D11DepthStencilState> depthStencilState;
        D3D_TRY(dxDevice->CreateDepthStencilState(&depthBufferDesc, &depthStencilState));

        dxContext->OMSetDepthStencilState(depthStencilState.Get(), 1u);

        SetViewport(window.GetWidth(), window.GetHeight());
    }

    void Instance::SetViewport(const UINT width, const UINT height, const UINT x, const UINT y)
    {
        D3D11_VIEWPORT viewport;
        viewport.Width = width;
        viewport.Height = height;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        viewport.TopLeftX = x;
        viewport.TopLeftY = y;
        dxContext->RSSetViewports(1u, &viewport);
    }

    void Instance::OnResize(const unsigned int, const unsigned int)
    {
        renderTargetView.reset();
        swapChain->Resize(window.GetWidth(), window.GetHeight());
        renderTargetView = CreateRef<DX::RenderTargetView>(swapChain.get(), true);
        SetViewport(window.GetWidth(), window.GetHeight());
    }

    void MakeInstanceCurrent(Instance* newContext)
    {
        CurrentInstance = newContext;
    }

    Instance* GetInstance(Window* window)
    {
        return Instances[window];
    }

#ifndef NDEBUG
    Debugger* GetDebugger()
    {
        return CurrentInstance->debugger.get();
    }
#endif

    ID3D11Device* GetDevice()
    {
        return CurrentInstance->dxDevice.Get();
    }

    ID3D11DeviceContext* GetContext()
    {
        return CurrentInstance->dxContext.Get();
    }

    SwapChain* GetSwapChain()
    {
        return CurrentInstance->swapChain.get();
    }

    Ref<RenderTargetView>& GetRenderTargetView()
    {
        return CurrentInstance->renderTargetView;
    }
}