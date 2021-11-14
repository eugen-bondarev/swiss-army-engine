#include "window.h"
#include "graphics.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "d3d/buffer.h"
#include "d3d/shader.h"

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

VertexBuffer* triangleVertexBuffer{nullptr};
IndexBuffer* triangleIndexBuffer{nullptr};
ConstantBuffer* triangleConstBuffer{nullptr};
Shader* triangleShader{nullptr};

const char* vertexShaderCode = R""""(

struct VSOut
{
    float3 color : Color;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    matrix transform;
};

VSOut main(float3 pos : Position)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform );
    vso.color = pos;
    return vso;
}

)"""";

const char* pixelShaderCode = R""""(

float4 main(float3 color : Color) : SV_Target
{
    return float4(color, 1.0f);
}

)"""";

void DrawTestTriangle(const float angleY, const float angleZ)
{
    triangleShader->Bind();

    triangleVertexBuffer->Bind(sizeof(Vertex), 0u);
    triangleIndexBuffer->Bind();
    triangleConstBuffer->Bind();

    dx::XMMATRIX transform = 
        dx::XMMatrixRotationX(angleZ) * 
        dx::XMMatrixRotationY(angleY) * 
        dx::XMMatrixRotationZ(angleZ) * 
        dx::XMMatrixTranslation(0, 0, 10) * 
        dx::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

    dx::XMMATRIX* data = (dx::XMMATRIX*) triangleConstBuffer->Map();
    *data = dx::XMMatrixTranspose(transform);
    triangleConstBuffer->Unmap();

    d3d->ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(d3d->ctx->DrawIndexed(indices.size(), 0u, 0u));
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
    // Graphics* graphics;

    try
    {
        Window wnd(800, 600, "WindowClass");

        // graphics = new Graphics(wnd.GetHandle());
        d3d = new D3D();
        d3d->Create(wnd.GetHandle());

        D3D11_VIEWPORT vp;
        vp.Width = 800;
        vp.Height = 600;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        d3d->ctx->RSSetViewports(1u, &vp);

        triangleVertexBuffer = new VertexBuffer(sizeof(Vertex) * vertices.size(), sizeof(Vertex), vertices.data());
        triangleIndexBuffer = new IndexBuffer(sizeof(uint32_t) * indices.size(), sizeof(uint32_t), indices.data());
        triangleConstBuffer = new ConstantBuffer(sizeof(dx::XMMATRIX));
        triangleShader = new Shader(vertexShaderCode, pixelShaderCode);

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
            d3d->ctx->ClearRenderTargetView(d3d->renderTargetView.Get(), clearColor.data());
            d3d->ctx->ClearDepthStencilView(d3d->depthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

            static float angle{0};
            DrawTestTriangle(angle, 0);
            DrawTestTriangle(0, angle);
            angle += 0.01f;

            HRESULT hr = d3d->swapchain->Present(1u, 0u);
            if (FAILED(hr))
            {
                if (hr == DXGI_ERROR_DEVICE_REMOVED)
                {
                    throw EXCEPTION_WHAT(std::to_string(d3d->device->GetDeviceRemovedReason()));
                }
                else
                {
                    throw EXCEPTION();
                }
            }
        }

        delete triangleShader;
        delete triangleConstBuffer;
        delete triangleIndexBuffer;
        delete triangleVertexBuffer;

        if (result == -1)
        {
            return result;
        }
    }
    catch (const std::runtime_error& p_exception)
    {
        MessageBox(nullptr, p_exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    // delete graphics;
    d3d->Shutdown();
    delete d3d;

    return 0;
}