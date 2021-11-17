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
        DXTexture = CreatePtr<Texture>(Width, Height, nullptr, true);
        D3D_TRY(GetDevice()->CreateRenderTargetView(DXTexture->GetDXTexture(), nullptr, &DXRenderTargetView));
    }
    else
    {
        DXTexture = CreatePtr<Texture>(Width, Height, nullptr, true);
        D3D_TRY(GetDevice()->CreateRenderTargetView(Resource, nullptr, &DXRenderTargetView));
    }

    if (Depth)
    {
        ComPtr<ID3D11Texture2D> depthTexture;
        D3D11_TEXTURE2D_DESC depthTextureDesc{};
        depthTextureDesc.Width = Width;
        depthTextureDesc.Height = Height;
        depthTextureDesc.MipLevels = 1u;
        depthTextureDesc.ArraySize = 1u;
        depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthTextureDesc.SampleDesc.Count = 1u;
        depthTextureDesc.SampleDesc.Quality = 0u;
        depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        D3D_TRY(GetDevice()->CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture));

        D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc{};
        depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthViewDesc.Texture2D.MipSlice = 0u;
        D3D_TRY(GetDevice()->CreateDepthStencilView(depthTexture.Get(), &depthViewDesc, &DXDepthView));
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
    GetContext()->OMSetRenderTargets(1u, DXRenderTargetView.GetAddressOf(), DXDepthView.Get());
}

void RenderTargetView::Unbind()
{
    GetContext()->OMSetRenderTargets(1u, nullptr, nullptr);
}

void RenderTargetView::Clear(const std::array<float, 4>& ClearColor)
{    
    GetContext()->ClearRenderTargetView(DXRenderTargetView.Get(), ClearColor.data());

    if (DXDepthView)
    {
        GetContext()->ClearDepthStencilView(DXDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    }
}

}