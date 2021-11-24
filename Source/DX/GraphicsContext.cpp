#include "GraphicsContext.h"

#include "../Window/RawWindow.h"
#include "RenderTargetView.h"

namespace DX
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc{};
        swapChainDesc.BufferDesc.Width = 0;
        swapChainDesc.BufferDesc.Height = 0;

        // swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        // swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferCount = 2;
        
        swapChainDesc.OutputWindow = glfwGetWin32Window(window.GetHandle());
        swapChainDesc.Windowed = TRUE;

        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        // swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        swapChain = CreatePtr<SwapChain>(window);
        window.ResizeSubscribe(std::bind(&GraphicsContext::OnResize, this, std::placeholders::_1));
        window.SetSwapChain(swapChain.get());

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
            &swapChain->dxSwapChain,
            &dxDevice,
            nullptr,
            &dxContext)
        );

#ifndef NDEBUG
        debugger = CreatePtr<Debugger>();
#endif

        renderTargetView = CreateRef<RenderTargetView>(swapChain.get(), true);
        renderTargetView->Bind();

        D3D11_DEPTH_STENCIL_DESC depthBufferDesc{};
        depthBufferDesc.DepthEnable = TRUE;
        depthBufferDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthBufferDesc.DepthFunc = D3D11_COMPARISON_LESS;
        ComPtr<ID3D11DepthStencilState> depthStencilState;
        D3D_TRY(dxDevice->CreateDepthStencilState(&depthBufferDesc, &depthStencilState));

        dxContext->OMSetDepthStencilState(depthStencilState.Get(), 1u);

        SetViewport(window.GetSize().x, window.GetSize().y);
    }

    API::Type GraphicsContext::GetAPIType() const
    {
        return API::Type::DirectX;
    }

    void GraphicsContext::SetViewport(const UINT width, const UINT height, const UINT x, const UINT y)
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

    void GraphicsContext::OnResize(const Vec2ui size)
    {
        renderTargetView.reset();
        swapChain->Resize(window.GetSize());
        renderTargetView = CreateRef<DX::RenderTargetView>(swapChain.get(), true);
        SetViewport(window.GetSize().x, window.GetSize().y);
    }

#ifndef NDEBUG
    Debugger* GetDebugger()
    {
        return dynamic_cast<DX::GraphicsContext*>(API::GetCurrentGraphicsContext())->debugger.get();
    }
#endif

    ID3D11Device* GetDevice()
    {
        return dynamic_cast<DX::GraphicsContext*>(API::GetCurrentGraphicsContext())->dxDevice.Get();
    }

    ID3D11DeviceContext* GetContext()
    {
        return dynamic_cast<DX::GraphicsContext*>(API::GetCurrentGraphicsContext())->dxContext.Get();
    }

    SwapChain* GetSwapChain()
    {
        return dynamic_cast<DX::GraphicsContext*>(API::GetCurrentGraphicsContext())->swapChain.get();
    }

    Ref<RenderTargetView>& GetRenderTargetView()
    {
        return dynamic_cast<DX::GraphicsContext*>(API::GetCurrentGraphicsContext())->renderTargetView;
    }
}