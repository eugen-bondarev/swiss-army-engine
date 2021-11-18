#ifndef __DX_TEXTURE_H__
#define __DX_TEXTURE_H__

#pragma once

#include "Instance.h"

namespace DX {

class Texture
{
public:
    Texture(const unsigned int Width, const unsigned int Height, const unsigned char* Data = nullptr, const UINT BindFlags = D3D11_BIND_SHADER_RESOURCE);
   ~Texture() = default;

    void Bind();

    ID3D11ShaderResourceView* GetDXView();
    ID3D11Texture2D* GetDXTexture();

private:
    ComPtr<ID3D11Texture2D>          DXTexture;
    ComPtr<ID3D11ShaderResourceView> DXView;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
};

}

#endif