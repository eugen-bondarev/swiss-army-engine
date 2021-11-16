#ifndef __DX_TEXTURE_H__
#define __DX_TEXTURE_H__

#pragma once

#include "instance.h"

namespace DX {

class Texture
{
public:
    Texture(const unsigned int Width, const unsigned int Height, const unsigned char* Data = nullptr, bool RenderTarget = false);
   ~Texture() = default;

    void Bind();

    ID3D11Texture2D* GetDXTexture()
    {
        return DXTexture.Get();
    }

    ID3D11ShaderResourceView* GetView()
    {
        return DXView.Get();
    }

private:
    ComPtr<ID3D11Texture2D>          DXTexture;
    ComPtr<ID3D11ShaderResourceView> DXView;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
};

}

#endif