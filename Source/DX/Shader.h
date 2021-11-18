#ifndef __DX_SHADER_H__
#define __DX_SHADER_H__

#pragma once

#include "Instance.h"

namespace DX
{
    class Shader
    {
    public:
        Shader(const std::string& VSCode, const std::string& PSCode);
       ~Shader() = default;

        void Bind();

    private:
        ComPtr<ID3D11InputLayout> DXInputLayout;
        ComPtr<ID3D11VertexShader> DXVertexShader;
        ComPtr<ID3D11PixelShader> DXPixelShader;

        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;
    };
}

#endif