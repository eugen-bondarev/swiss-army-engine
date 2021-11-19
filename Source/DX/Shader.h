#ifndef __DX_Shader_h__
#define __DX_Shader_h__

#pragma once

#include "Instance.h"

namespace DX
{
    class Shader
    {
    public:
        Shader(const std::string& vsCode, const std::string& psCode);
       ~Shader() = default;

        void Bind();

    private:
        ComPtr<ID3D11InputLayout> dxInputLayout;
        ComPtr<ID3D11VertexShader> dxVertexShader;
        ComPtr<ID3D11PixelShader> dxPixelShader;

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
    };
}

#endif