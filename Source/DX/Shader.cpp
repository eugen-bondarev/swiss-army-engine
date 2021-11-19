#include "Shader.h"

#include <d3dcompiler.h>

namespace DX
{
    Shader::Shader(const std::string &vsCode, const std::string &psCode)
    {
        ComPtr<ID3D10Blob> vsBlob;
        ComPtr<ID3D10Blob> psBlob;
        ComPtr<ID3D10Blob> errorBlob;

        if (FAILED(
                D3DCompile(
                    std::string(vsCode).c_str(),
                    std::string(vsCode).length(),
                    nullptr, nullptr, nullptr,
                    "main", "vs_5_0",
                    D3DCOMPILE_ENABLE_STRICTNESS, 0,
                    vsBlob.GetAddressOf(),
                    errorBlob.GetAddressOf())
            ) && errorBlob.GetAddressOf())
        {
            throw std::runtime_error(std::string(static_cast<char*>(errorBlob->GetBufferPointer())));
        }

        if (FAILED(
                D3DCompile(
                    std::string(psCode).c_str(),
                    std::string(psCode).length(),
                    nullptr, nullptr, nullptr,
                    "main", "ps_5_0",
                    D3DCOMPILE_ENABLE_STRICTNESS, 0,
                    psBlob.GetAddressOf(),
                    errorBlob.GetAddressOf())
            ) && errorBlob.GetAddressOf())
        {
            throw std::runtime_error(std::string(static_cast<char*>(errorBlob->GetBufferPointer())));
        }

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout =
        {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TexCoords", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        D3D_TRY(GetDevice()->CreateInputLayout(inputLayout.data(), inputLayout.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &dxInputLayout));
        D3D_TRY(GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &dxVertexShader));
        D3D_TRY(GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &dxPixelShader));
    }

    void Shader::Bind()
    {
        GetContext()->IASetInputLayout(dxInputLayout.Get());
        GetContext()->VSSetShader(dxVertexShader.Get(), nullptr, 0u);
        GetContext()->PSSetShader(dxPixelShader.Get(), nullptr, 0u);
    }
}