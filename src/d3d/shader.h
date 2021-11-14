#ifndef __D3D_SHADER_H__
#define __D3D_SHADER_H__

#pragma once

#include "common.h"
// #include "../graphics.h"
#include "d3d.h"

class Shader
{
public:
    Shader(const std::string& vertexShaderCode, const std::string& pixelShaderCode)
    {        
        ID3D10Blob* vsBlob;
        ID3D10Blob* psBlob;
        ID3D10Blob* errorBlob;

        HRESULT hr;

        hr = D3DCompile(
            std::string(vertexShaderCode).c_str(),
            std::string(vertexShaderCode).length(),
            nullptr,
            nullptr,
            nullptr,
            "main", "vs_5_0",
            D3DCOMPILE_ENABLE_STRICTNESS, 0,
            &vsBlob,
            &errorBlob
        );

        if (FAILED(hr))
        {
            if (errorBlob)
            {
                OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
                errorBlob->Release();
            }
        }

        hr = D3DCompile(
            std::string(pixelShaderCode).c_str(),
            std::string(pixelShaderCode).length(),
            nullptr,
            nullptr,
            nullptr,
            "main", "ps_5_0",
            D3DCOMPILE_ENABLE_STRICTNESS, 0,
            &psBlob,
            &errorBlob
        );
        
        if (FAILED(hr))
        {
            if (errorBlob)
            {
                OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
                errorBlob->Release();
            }
        }

        std::vector<D3D11_INPUT_ELEMENT_DESC> ied = 
        {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        D3D_TRY(d3d->device->CreateInputLayout(
            ied.data(), ied.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout
        ));

        D3D_CHECK(d3d->device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader));
        D3D_CHECK(d3d->device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader));
    }

    void Bind()
    {        
        d3d->ctx->IASetInputLayout(inputLayout.Get());
        d3d->ctx->VSSetShader(vertexShader.Get(), nullptr, 0u);
        d3d->ctx->PSSetShader(pixelShader.Get(), nullptr, 0u);
    }

    ComPtr<ID3D11InputLayout> inputLayout;
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
};

#endif