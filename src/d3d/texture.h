#ifndef __D3D_TEXTURE_H__
#define __D3D_TEXTURE_H__

#pragma once

#include "common.h"
#include "d3d.h"

class Texture
{
public:
    Texture(const unsigned int Width, const unsigned int Height, const unsigned char* Data)
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
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA textureSubData{};
        textureSubData.pSysMem = Data;
        textureSubData.SysMemPitch = Width * sizeof(unsigned char) * 4;

        ComPtr<ID3D11Texture2D> texture;
        D3D_CHECK(Device()->CreateTexture2D(&textureDesc, &textureSubData, &texture));

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        D3D_CHECK(Device()->CreateShaderResourceView(texture.Get(), &srvDesc, &View));
    }

   ~Texture() = default;

    ComPtr<ID3D11ShaderResourceView> View;

    void Bind()
    {
        Ctx()->PSSetShaderResources(0u, 1u, View.GetAddressOf());
    }

private:
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
};

#endif