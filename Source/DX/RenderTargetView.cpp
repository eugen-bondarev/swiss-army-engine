#include "RenderTargetView.h"

namespace DX {

RenderTargetView::RenderTargetView(SwapChain* Swapchain, const bool Depth)
{
    ComPtr<ID3D11Resource> backBuffer{nullptr};
    D3D_TRY(Swapchain->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
    Init(Swapchain->GetWidth(), Swapchain->GetHeight(), backBuffer.Get(), Depth);
}

RenderTargetView::RenderTargetView(const unsigned int Width, const unsigned int Height, const bool Depth)
{
    Init(Width, Height, nullptr, Depth);
}

void RenderTargetView::Init(const unsigned int Width, const unsigned int Height, ID3D11Resource* Resource, const bool Depth)
{
    if (!Resource)
    {
        DXTexture = CreatePtr<Texture>(Width, Height, nullptr, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
        D3D_TRY(GetDevice()->CreateRenderTargetView(DXTexture->GetDXTexture(), nullptr, &DXRenderTargetView));
    }
    else
    {
        D3D_TRY(GetDevice()->CreateRenderTargetView(Resource, nullptr, &DXRenderTargetView));
    }

    if (Depth)
    {
        DXDepthBuffer = CreatePtr<DepthBuffer>(Width, Height);
    }
}

Texture* RenderTargetView::GetTexture()
{
    return DXTexture.get();
}

ID3D11RenderTargetView* RenderTargetView::GetDXRenderTarget()
{
    return DXRenderTargetView.Get();
}

void RenderTargetView::Bind()
{
    GetContext()->OMSetRenderTargets(1u, DXRenderTargetView.GetAddressOf(), DXDepthBuffer ? DXDepthBuffer->GetDXDepthStencilView() : nullptr);
}

void RenderTargetView::Unbind()
{
    GetContext()->OMSetRenderTargets(1u, nullptr, nullptr);
}

void RenderTargetView::Clear(const std::array<float, 4>& ClearColor)
{    
    GetContext()->ClearRenderTargetView(DXRenderTargetView.Get(), ClearColor.data());

    if (DXDepthBuffer)
    {
        DXDepthBuffer->Clear();
    }
}

}