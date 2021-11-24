#include "DepthBuffer.h"

namespace DX 
{
    DepthBuffer::DepthBuffer(const Vec2ui size)
    {
        D3D11_TEXTURE2D_DESC depthTextureDesc{};
        depthTextureDesc.Width = size.x;
        depthTextureDesc.Height = size.y;
        depthTextureDesc.MipLevels = 1u;
        depthTextureDesc.ArraySize = 1u;
        depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthTextureDesc.SampleDesc.Count = 1u;
        depthTextureDesc.SampleDesc.Quality = 0u;
        depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        
        ComPtr<ID3D11Texture2D> depthTexture;
        D3D_TRY(GetDevice()->CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture));

        D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc{};
        depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthViewDesc.Texture2D.MipSlice = 0u;
        D3D_TRY(GetDevice()->CreateDepthStencilView(depthTexture.Get(), &depthViewDesc, &dxDepthView));
    }

    void DepthBuffer::Clear()
    {
        GetContext()->ClearDepthStencilView(dxDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    }

    ID3D11DepthStencilView* DepthBuffer::GetDXDepthStencilView()
    {
        return dxDepthView.Get();
    }
}