#include "Texture.h"

namespace DX
{
    Texture::Texture(const Vec2ui size, const unsigned char* data, const UINT bindFlags)
    {
        D3D11_TEXTURE2D_DESC textureDesc{};
        textureDesc.Width = size.x;
        textureDesc.Height = size.y;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = bindFlags;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        if (data)
        {
            D3D11_SUBRESOURCE_DATA textureSubData{};
            textureSubData.pSysMem = data;
            textureSubData.SysMemPitch = size.x * sizeof(unsigned char) * 4;
            D3D_TRY(GetDevice()->CreateTexture2D(&textureDesc, &textureSubData, &dxTexture));
        }
        else
        {
            D3D_TRY(GetDevice()->CreateTexture2D(&textureDesc, nullptr, &dxTexture));
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        D3D_TRY(GetDevice()->CreateShaderResourceView(dxTexture.Get(), &srvDesc, &dxView));
    }

    void Texture::Bind()
    {
        GetContext()->PSSetShaderResources(0u, 1u, dxView.GetAddressOf());
    }

    ID3D11Texture2D* Texture::GetDXTexture()
    {
        return dxTexture.Get();
    }

    ID3D11ShaderResourceView* Texture::GetDXView()
    {
        return dxView.Get();
    }
}