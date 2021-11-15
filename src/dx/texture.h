#ifndef __DX_TEXTURE_H__
#define __DX_TEXTURE_H__

#pragma once

#include "instance.h"

namespace DX {

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

}

#endif