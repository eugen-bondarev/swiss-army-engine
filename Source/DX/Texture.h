#ifndef __DX_Texture_h__
#define __DX_Texture_h__

#pragma once

#include "Instance.h"

namespace DX
{
    class Texture
    {
    public:
        Texture(const unsigned int width, const unsigned int height, const unsigned char* data = nullptr, const UINT bindFlags = D3D11_BIND_SHADER_RESOURCE);
       ~Texture() = default;

        void Bind();

        ID3D11Texture2D* GetDXTexture();
        ID3D11ShaderResourceView* GetDXView();

    private:
        ComPtr<ID3D11Texture2D> dxTexture;
        ComPtr<ID3D11ShaderResourceView> dxView;

        Texture(const Texture&) = delete;
        Texture &operator=(const Texture&) = delete;
    };
}

#endif