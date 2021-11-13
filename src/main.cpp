#include "window.h"
#include "graphics.h"

#include <d3dcompiler.h>

void DrawTestTriangle(Graphics* ctx)
{
    struct Vertex
    {
        float x, y;
    };

    std::vector<Vertex> vertices = 
    {
        {  0.0f,  0.5f },
        {  0.5f, -0.5f },
        { -0.5f, -0.5f }
    };

    ComPtr<ID3D11Buffer> vertexBuffer;
    
    D3D11_BUFFER_DESC bd{};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(Vertex) * vertices.size();
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertices.data();

    D3D_CHECK(ctx->device->CreateBuffer(&bd, &sd, &vertexBuffer));

    const UINT stride{sizeof(Vertex)};
    const UINT offset{0u};

    ctx->context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
    
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
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
    }

    ComPtr<ID3D11InputLayout> inputLayout;
    std::vector<D3D11_INPUT_ELEMENT_DESC> ied = 
    {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    D3D_CHECK(ctx->device->CreateInputLayout(
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
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
    }

    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    D3D_CHECK(ctx->device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader));
    D3D_CHECK(ctx->device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader));

    ctx->context->VSSetShader(vertexShader.Get(), nullptr, 0u);
    ctx->context->PSSetShader(pixelShader.Get(), nullptr, 0u);

    ctx->context->OMSetRenderTargets(1u, ctx->renderTargetView.GetAddressOf(), nullptr);

    ctx->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    ctx->context->RSSetViewports(1u, &vp);

    D3D_TRY(ctx->context->Draw(vertices.size(), 0u));
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
    MSG msg{};

    Graphics* graphics;

    try
    {
        Window wnd(800, 600, "WindowClass");

        graphics = new Graphics(wnd.GetHandle());

        BOOL result;
        while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            const static std::array<float, 4> clearColor{0.2f, 1.0f, 0.5f, 1.0f};
            graphics->context->ClearRenderTargetView(graphics->renderTargetView.Get(), clearColor.data());

            // Render here
            DrawTestTriangle(graphics);

            HRESULT hr;
            if (FAILED(hr = graphics->swapchain->Present(1u, 0u)))
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
        // LINE_OUT(p_exception.what());
    }

    delete graphics;

    return msg.wParam;
}