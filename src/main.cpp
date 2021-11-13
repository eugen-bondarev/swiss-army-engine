#include "window.h"
#include "graphics.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "d3d/buffer.h"

struct Vertex
{
    float x, y, z;
};

std::vector<Vertex> vertices = 
{
    { -1, -1, -1 },
    { 1, -1, -1 },
    { -1, 1, -1 },
    { 1, 1, -1 },
    { -1, -1, 1 },
    { 1, -1, 1 },
    { -1, 1, 1 },
    { 1, 1, 1 },
};

std::vector<uint32_t> indices = 
{
    0, 2, 1,  2, 3, 1,
    1, 3, 5,  3, 7, 5,
    2, 6, 3,  3, 6, 7,
    4, 5, 7,  4, 7, 6,
    0, 4, 2,  2, 4, 6,
    0, 1, 4,  1, 5, 4,
};

namespace dx = DirectX;

void DrawTestTriangle(Graphics* ctx, const float angleY, const float angleZ)
{
    Buffer vertexBuffer(sizeof(Vertex) * vertices.size(), sizeof(Vertex), vertices.data(), D3D11_BIND_VERTEX_BUFFER);
    Buffer indexBuffer(sizeof(uint32_t) * indices.size(), sizeof(uint32_t), indices.data(), D3D11_BIND_INDEX_BUFFER);

	ID3D10Blob* vsBlob;
	ID3D10Blob* psBlob;
	ID3D10Blob* errorBlob;

    HRESULT hr;

    hr = D3DCompileFromFile(
        L"C:/Users/azare/Documents/Dev/Cpp/direct3d/vertex-shader.hlsl",
        nullptr, nullptr,
        "main", "vs_5_0",
        0u, 0u,
        &vsBlob, &errorBlob
    );
    
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
    }

    ComPtr<ID3D11InputLayout> inputLayout;
    std::vector<D3D11_INPUT_ELEMENT_DESC> ied = 
    {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    D3D_TRY(ctx->device->CreateInputLayout(
        ied.data(), ied.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout
    ));

    ctx->context->IASetInputLayout(inputLayout.Get());

    hr = D3DCompileFromFile(
        L"C:/Users/azare/Documents/Dev/Cpp/direct3d/pixel-shader.hlsl",
        nullptr, nullptr,
        "main", "ps_5_0",
        0u, 0u,
        &psBlob, &errorBlob
    );
    
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
    }

    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    D3D_CHECK(ctx->device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader));
    D3D_CHECK(ctx->device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader));

    vertexBuffer.BindVertexBuffer(sizeof(Vertex), 0u);
    indexBuffer.BindIndexBuffer();

    // Constant buffer
    struct ConstantBuffer
    {
        dx::XMMATRIX transform;
    };

    dx::XMMATRIX transform = 
        dx::XMMatrixRotationX(angleZ) * 
        dx::XMMatrixRotationY(angleY) * 
        dx::XMMatrixRotationZ(angleZ) * 
        dx::XMMatrixTranslation(0, 0, 10) * 
        dx::XMMatrixPerspectiveFovLH(70.0f * 3.14159265359f / 180.0f, 4.0f / 3.0f, 0.1f, 1000.0f)
    ;

    const ConstantBuffer cb =
    {
        {
            dx::XMMatrixTranspose(transform)
        }
    };

    ComPtr<ID3D11Buffer> constantBuffer;
    D3D11_BUFFER_DESC constantBufferDesc{};
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufferDesc.MiscFlags = 0u;
    constantBufferDesc.ByteWidth = sizeof(cb);
    constantBufferDesc.StructureByteStride = 0u;
    
    D3D11_SUBRESOURCE_DATA constantBufferSubData{};
    constantBufferSubData.pSysMem = &cb;
    
    D3D_TRY(ctx->device->CreateBuffer(&constantBufferDesc, &constantBufferSubData, &constantBuffer));

    ctx->context->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
    ///////

    ctx->context->VSSetShader(vertexShader.Get(), nullptr, 0u);
    ctx->context->PSSetShader(pixelShader.Get(), nullptr, 0u);

    // ctx->context->OMSetRenderTargets(1u, ctx->renderTargetView.GetAddressOf(), nullptr);

    ctx->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    ctx->context->RSSetViewports(1u, &vp);

    D3D_TRY(ctx->context->DrawIndexed(indices.size(), 0u, 0u));
}

#define USE_CONSOLE

#ifndef USE_CONSOLE
#   define INSTANCE() pHinstance
    int CALLBACK WinMain(HINSTANCE INSTANCE(), HINSTANCE pPrevHinstance, LPSTR pLpCmdLine, int pCmdShow)
#else
#   define INSTANCE() GetModuleHandle(nullptr)
    int main()
#endif
{

    Graphics* graphics;

    try
    {
        Window wnd(800, 600, "WindowClass");

        graphics = new Graphics(wnd.GetHandle());

        BOOL result;
        while (true)
        {
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    return static_cast<int>(msg.wParam);
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            const static std::array<float, 4> clearColor{0.2f, 1.0f, 0.5f, 1.0f};
            graphics->context->ClearRenderTargetView(graphics->renderTargetView.Get(), clearColor.data());
            graphics->context->ClearDepthStencilView(graphics->depthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

            static float angle{0};
            DrawTestTriangle(graphics, angle, 0);
            DrawTestTriangle(graphics, 0, angle);
            angle += 0.01f;

            HRESULT hr = graphics->swapchain->Present(1u, 0u);
            if (FAILED(hr))
            {
                if (hr == DXGI_ERROR_DEVICE_REMOVED)
                {
                    throw EXCEPTION_WHAT(std::to_string(graphics->device->GetDeviceRemovedReason()));
                }
                else
                {
                    throw EXCEPTION();
                }
            }
        }

        if (result == -1)
        {
            return result;
        }
    }
    catch (const std::runtime_error& p_exception)
    {
        MessageBox(nullptr, p_exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    delete graphics;

    return 0;
}