#include "RenderTargetView.h"

namespace DX {

RenderTargetView::RenderTargetView(ID3D11Resource* Resource, bool Depth)
{
    if (!Resource)
    {
        DXTexture = CreatePtr<Texture>(1920u, 1080u, nullptr, true);
        D3D_TRY(GetDevice()->CreateRenderTargetView(DXTexture->GetDXTexture(), nullptr, &DXRenderTargetView));
    }
    else
    {
        DXTexture = CreatePtr<Texture>(1920u, 1080u, nullptr, true);
        D3D_TRY(GetDevice()->CreateRenderTargetView(Resource, nullptr, &DXRenderTargetView));
    }

    if (Depth)
    {
        ComPtr<ID3D11Texture2D> depthTexture;
        D3D11_TEXTURE2D_DESC depthTextureDesc{};
        depthTextureDesc.Width = 1920u;
        depthTextureDesc.Height = 1080u;
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

    if (DXDepthView)
    {
        GetContext()->ClearDepthStencilView(DXDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    }
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