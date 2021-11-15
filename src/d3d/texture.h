#ifndef __D3D_TEXTURE_H__
#define __D3D_TEXTURE_H__

#pragma once

#include "dx_instance.h"

class Texture
{
public:
    Texture(const unsigned int Width, const unsigned int Height, const unsigned char* Data);
   ~Texture() = default;

    void Bind();

private:
    ComPtr<ID3D11ShaderResourceView> DXView;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
};

#endif