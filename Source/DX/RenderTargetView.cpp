#include "RenderTargetView.h"

namespace DX {

RenderTargetView::RenderTargetView(SwapChain* swapChain, const bool initDepth)
{
    ComPtr<ID3D11Resource> backBuffer{nullptr};
    D3D_TRY(swapChain->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
    Init(swapChain->GetWidth(), swapChain->GetHeight(), backBuffer.Get(), initDepth);
}

RenderTargetView::RenderTargetView(const unsigned int width, const unsigned int height, const bool initDepth)
{
    Init(width, height, nullptr, initDepth);
}

void RenderTargetView::Init(const unsigned int width, const unsigned int height, ID3D11Resource* resource, const bool initDepth)
{
    if (!resource)
    {
        texture = CreatePtr<Texture>(width, height, nullptr, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
        D3D_TRY(GetDevice()->CreateRenderTargetView(texture->GetDXTexture(), nullptr, &dxRenderTargetView));
    }
    else
    {
        D3D_TRY(GetDevice()->CreateRenderTargetView(resource, nullptr, &dxRenderTargetView));
    }

    if (initDepth)
    {
        depthBuffer = CreatePtr<DepthBuffer>(width, height);
    }
}

Texture* RenderTargetView::GetTexture()
{
    return texture.get();
}

ID3D11RenderTargetView* RenderTargetView::GetDXRenderTarget()
{
    return dxRenderTargetView.Get();
}

void RenderTargetView::Bind()
{
    GetContext()->OMSetRenderTargets(1u, dxRenderTargetView.GetAddressOf(), depthBuffer ? depthBuffer->GetDXDepthStencilView() : nullptr);
}

void RenderTargetView::Unbind()
{
    GetContext()->OMSetRenderTargets(1u, nullptr, nullptr);
}

void RenderTargetView::Clear(const std::array<float, 4>& clearColor)
{    
    GetContext()->ClearRenderTargetView(dxRenderTargetView.Get(), clearColor.data());

    if (depthBuffer)
    {
        depthBuffer->Clear();
    }
}

}