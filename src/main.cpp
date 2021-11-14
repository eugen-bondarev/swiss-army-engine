#include <d3dcompiler.h>
#include <DirectXMath.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "d3d/window.h"
#include "d3d/buffer.h"
#include "d3d/shader.h"

struct Vertex
{
    float x, y, z;
};

std::vector<Vertex> cubeVertices = 
{
    { -1, -1, -1 },
    {  1, -1, -1 },
    { -1,  1, -1 },
    {  1,  1, -1 },
    { -1, -1,  1 },
    {  1, -1,  1 },
    { -1,  1,  1 },
    {  1,  1,  1 },
};

std::vector<uint32_t> cubeIndices = 
{
    0, 2, 1,  2, 3, 1,
    1, 3, 5,  3, 7, 5,
    2, 6, 3,  3, 6, 7,
    4, 5, 7,  4, 7, 6,
    0, 4, 2,  2, 4, 6,
    0, 1, 4,  1, 5, 4,
};

namespace dx = DirectX;

VertexBuffer*   triangleVertexBuffer1{nullptr};
IndexBuffer*    triangleIndexBuffer1{nullptr};
ConstantBuffer* triangleConstBuffer1{nullptr};
Shader*         triangleShader1{nullptr};

VertexBuffer*   triangleVertexBuffer2{nullptr};
IndexBuffer*    triangleIndexBuffer2{nullptr};
ConstantBuffer* triangleConstBuffer2{nullptr};
Shader*         triangleShader2{nullptr};

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

void DrawTestTriangle1(const float angleY, const float angleZ)
{
    triangleShader1->Bind();
    triangleVertexBuffer1->Bind(sizeof(Vertex), 0u);
    triangleIndexBuffer1->Bind();
    triangleConstBuffer1->Bind();

    dx::XMMATRIX transform = 
        dx::XMMatrixRotationX(angleZ) * 
        dx::XMMatrixRotationY(angleY) * 
        dx::XMMatrixRotationZ(angleZ) * 
        dx::XMMatrixTranslation(0, 0, 10) * 
        dx::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

    dx::XMMATRIX* data = (dx::XMMATRIX*) triangleConstBuffer1->Map();
    *data = dx::XMMatrixTranspose(transform);
    triangleConstBuffer1->Unmap();

    Ctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(Ctx()->DrawIndexed(cubeIndices.size(), 0u, 0u));
}

void DrawTestTriangle2(const float angleY, const float angleZ)
{
    triangleShader2->Bind();
    triangleVertexBuffer2->Bind(sizeof(Vertex), 0u);
    triangleIndexBuffer2->Bind();
    triangleConstBuffer2->Bind();

    dx::XMMATRIX transform = 
        dx::XMMatrixRotationX(angleZ) * 
        dx::XMMatrixRotationY(angleY) * 
        dx::XMMatrixRotationZ(angleZ) * 
        dx::XMMatrixTranslation(0, 0, 10) * 
        dx::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

    dx::XMMATRIX* data = static_cast<dx::XMMATRIX*>(triangleConstBuffer2->Map());
    *data = dx::XMMatrixTranspose(transform);
    triangleConstBuffer2->Unmap();

    Ctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(Ctx()->DrawIndexed(cubeIndices.size(), 0u, 0u));
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
    D3D* ctx1;
    D3D* ctx2;

    try
    {
        Window window1;
        Window window2;

        ctx1 = new D3D(glfwGetWin32Window(window1.handle));
        ctx2 = new D3D(glfwGetWin32Window(window2.handle));

        D3D11_VIEWPORT viewport;
        viewport.Width = 800;
        viewport.Height = 600;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;

        Ctx()->RSSetViewports(1u, &viewport);
        triangleVertexBuffer2 = new VertexBuffer(sizeof(Vertex) * cubeVertices.size(), sizeof(Vertex), cubeVertices.data());
        triangleIndexBuffer2 = new IndexBuffer(sizeof(uint32_t) * cubeIndices.size(), sizeof(uint32_t), cubeIndices.data());
        triangleConstBuffer2 = new ConstantBuffer(sizeof(dx::XMMATRIX));
        triangleShader2 = new Shader(vertexShaderCode, pixelShaderCode);

        MakeContextCurrent(ctx1);

        Ctx()->RSSetViewports(1u, &viewport);
        triangleVertexBuffer1 = new VertexBuffer(sizeof(Vertex) * cubeVertices.size(), sizeof(Vertex), cubeVertices.data());
        triangleIndexBuffer1 = new IndexBuffer(sizeof(uint32_t) * cubeIndices.size(), sizeof(uint32_t), cubeIndices.data());
        triangleConstBuffer1 = new ConstantBuffer(sizeof(dx::XMMATRIX));
        triangleShader1 = new Shader(vertexShaderCode, pixelShaderCode);

        while (!glfwWindowShouldClose(window1.handle) && !glfwWindowShouldClose(window2.handle))
        {
            glfwPollEvents();
            
            static float angle{0}; angle += 0.05f;
            const static std::array<float, 4> clearColor1{0.2f, 1.0f, 0.5f, 1.0f};
            const static std::array<float, 4> clearColor2{0.6f, 0.8f, 0.2f, 1.0f};

            MakeContextCurrent(ctx1);
            Ctx()->ClearRenderTargetView(RenderTargetView(), clearColor1.data());
            Ctx()->ClearDepthStencilView(DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
            DrawTestTriangle1(angle, 0);
            
            HRESULT hr = Swapchain()->Present(1u, 0u);            
            if (FAILED(hr))
            {
                if (hr == DXGI_ERROR_DEVICE_REMOVED)
                {
                    throw EXCEPTION_WHAT(std::to_string(Device()->GetDeviceRemovedReason()));
                }
                else
                {
                    throw EXCEPTION();
                }
            }

            MakeContextCurrent(ctx2);
            Ctx()->ClearRenderTargetView(RenderTargetView(), clearColor2.data());
            Ctx()->ClearDepthStencilView(DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
            DrawTestTriangle2(0, angle);
            
            hr = Swapchain()->Present(1u, 0u);            
            if (FAILED(hr))
            {
                if (hr == DXGI_ERROR_DEVICE_REMOVED)
                {
                    throw EXCEPTION_WHAT(std::to_string(Device()->GetDeviceRemovedReason()));
                }
                else
                {
                    throw EXCEPTION();
                }
            }
        }
    }
    catch (const std::runtime_error& p_exception)
    {
        MessageBox(nullptr, p_exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    delete triangleShader1;
    delete triangleConstBuffer1;
    delete triangleIndexBuffer1;
    delete triangleVertexBuffer1;

    delete triangleShader2;
    delete triangleConstBuffer2;
    delete triangleIndexBuffer2;
    delete triangleVertexBuffer2;
    
    delete ctx1;
    delete ctx2;

    return 0;
}