#include "RenderTargetView.h"

namespace DX {

RenderTargetView::RenderTargetView(SwapChain* swapChain, const bool Depth)
{
    ComPtr<ID3D11Resource> backBuffer{nullptr};
    D3D_TRY(swapChain->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
    Init(swapChain->GetWidth(), swapChain->GetHeight(), backBuffer.Get(), Depth);
}

RenderTargetView::RenderTargetView(const unsigned int width, const unsigned int height, const bool initDepth)
{
    Init(width, height, nullptr, initDepth);
}

void RenderTargetView::Init(const unsigned int width, const unsigned int height, ID3D11Resource* resource, const bool initDepth)
{
    if (!resource)
    {
        DXTexture = CreatePtr<Texture>(width, height, nullptr, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
        D3D_TRY(GetDevice()->CreateRenderTargetView(DXTexture->GetDXTexture(), nullptr, &DXRenderTargetView));
    }
    else
    {
        D3D_TRY(GetDevice()->CreateRenderTargetView(resource, nullptr, &DXRenderTargetView));
    }

    if (initDepth)
    {
        DXDepthBuffer = CreatePtr<DepthBuffer>(width, height);
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

void RenderTargetView::Clear(const std::array<float, 4>& clearColor)
{    
    GetContext()->ClearRenderTargetView(DXRenderTargetView.Get(), clearColor.data());

    if (DXDepthBuffer)
    {
        DXDepthBuffer->Clear();
    }
}

}