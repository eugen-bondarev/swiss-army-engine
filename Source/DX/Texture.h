#ifndef __DX_Texture_h__
#define __DX_Texture_h__

#pragma once

#include "GraphicsContext.h"
#include "../API/Texture.h"

namespace DX
{
    class Texture : public API::Texture
    {
    public:
        Texture(const Vec2ui size, const unsigned char* data = nullptr, const UINT bindFlags = D3D11_BIND_SHADER_RESOURCE, Device& device = GetDevice());
       ~Texture() = default;

        void Bind() override;

        ID3D11Texture2D* GetDXTexture();
        ID3D11ShaderResourceView* GetDXView();

    private:
        Device& device;

        ComPtr<ID3D11Texture2D> dxTexture;
        ComPtr<ID3D11ShaderResourceView> dxView;

        Texture(const Texture&) = delete;
        Texture &operator=(const Texture&) = delete;
    };
}

#endif