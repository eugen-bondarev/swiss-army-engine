#include "texture.h"

namespace DX {

Texture::Texture(const unsigned int Width, const unsigned int Height, const unsigned char* Data, bool RenderTarget)
{
    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.Width = Width;
    textureDesc.Height = Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = RenderTarget ? (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET) : (D3D11_BIND_SHADER_RESOURCE);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // ComPtr<ID3D11Texture2D> texture;

    if (Data)
    {
        D3D11_SUBRESOURCE_DATA textureSubData{};
        textureSubData.pSysMem = Data;
        textureSubData.SysMemPitch = Width * sizeof(unsigned char) * 4;
        D3D_TRY(GetDevice()->CreateTexture2D(&textureDesc, &textureSubData, &DXTexture));
    }
    else
    {
        D3D_TRY(GetDevice()->CreateTexture2D(&textureDesc, nullptr, &DXTexture));
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    D3D_TRY(GetDevice()->CreateShaderResourceView(DXTexture.Get(), &srvDesc, &DXView));
}

void Texture::Bind()
{
    GetContext()->PSSetShaderResources(0u, 1u, DXView.GetAddressOf());
}

}