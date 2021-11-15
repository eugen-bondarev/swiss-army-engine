#include "shader.h"

#include <d3dcompiler.h>

namespace DX {

Shader::Shader(const std::string& VSCode, const std::string& PSCode)
{        
    ComPtr<ID3D10Blob> vsBlob;
    ComPtr<ID3D10Blob> psBlob;
    ComPtr<ID3D10Blob> errorBlob;
    
    if (FAILED(
        D3DCompile(
            std::string(VSCode).c_str(),
            std::string(VSCode).length(),
            nullptr, nullptr, nullptr,
            "main", "vs_5_0",
            D3DCOMPILE_ENABLE_STRICTNESS, 0,
            vsBlob.GetAddressOf(),
            errorBlob.GetAddressOf()
        )
    ) && errorBlob.GetAddressOf())
    {
        throw std::runtime_error(std::string(static_cast<char*>(errorBlob->GetBufferPointer())));
    }
    
    if (FAILED(
        D3DCompile(
            std::string(PSCode).c_str(),
            std::string(PSCode).length(),
            nullptr, nullptr, nullptr,
            "main", "ps_5_0",
            D3DCOMPILE_ENABLE_STRICTNESS, 0,
            psBlob.GetAddressOf(),
            errorBlob.GetAddressOf()
        )
    ) && errorBlob.GetAddressOf())
    {
        throw std::runtime_error(std::string(static_cast<char*>(errorBlob->GetBufferPointer())));
    }

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout = 
    {
        { "Position",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TexCoords", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    D3D_TRY(GetDevice()->CreateInputLayout(
        inputLayout.data(), inputLayout.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &DXInputLayout
    ));

    D3D_TRY(GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &DXVertexShader));
    D3D_TRY(GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &DXPixelShader));
}

void Shader::Bind()
{        
    GetContext()->IASetInputLayout(DXInputLayout.Get());
    GetContext()->VSSetShader(DXVertexShader.Get(), nullptr, 0u);
    GetContext()->PSSetShader(DXPixelShader.Get(), nullptr, 0u);
}

}