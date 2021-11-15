#include <d3dcompiler.h>
#include <DirectXMath.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "d3d/window.h"
#include "d3d/buffer.h"
#include "d3d/shader.h"
#include "d3d/texture.h"
#include "d3d/sampler.h"

#include "util/assets.h"

#include <iostream>

namespace dx = DirectX;

struct Vertex
{
    struct
    {
        float x, y, z;
    } position;

    struct
    {
        float x, y;
    } texCoords;
};

std::vector<Vertex> cubeVertices = 
{
    { { -1,  1, 0 }, { 0, 0 } },
    { {  1,  1, 0 }, { 1, 0 } },
    { {  1, -1, 0 }, { 1, 1 } },
    { { -1, -1, 0 }, { 0, 1 } }
};

std::vector<uint32_t> cubeIndices = 
{
    0, 1, 2,  0, 2, 3
};

VertexBuffer*   meshVertexBuffer{nullptr};
IndexBuffer*    meshIndexBuffer{nullptr};
ConstantBuffer* constantBuffer{nullptr};
Shader*         shader{nullptr};
Sampler*        sampler{nullptr};
Texture*        texture{nullptr};

void DrawTestTriangle1(const float angleY, const float angleZ)
{
    shader->Bind();
    meshVertexBuffer->Bind(sizeof(Vertex), 0u);
    meshIndexBuffer->Bind();
    constantBuffer->Bind();
    sampler->Bind();
    texture->Bind();

    dx::XMMATRIX transform = 
        dx::XMMatrixRotationX(angleZ) * 
        dx::XMMatrixRotationY(angleY) * 
        dx::XMMatrixRotationZ(angleZ) * 
        dx::XMMatrixTranslation(0, 0, 3) * 
        dx::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

    dx::XMMATRIX* data = (dx::XMMATRIX*) constantBuffer->Map();
    *data = dx::XMMatrixTranspose(transform);
    constantBuffer->Unmap();

    Ctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(Ctx()->DrawIndexed(cubeIndices.size(), 0u, 0u));
}

int main()
{
    Window* window{nullptr};
    D3D11Instance* d3d{nullptr};

    try
    {
        window = new Window();

        d3d = new D3D11Instance(glfwGetWin32Window(window->handle));
        D3D11Instance::SetViewport(0, 0, 800, 600);

        const Util::TextAsset vertexShaderCode = Util::LoadTextFile("C:/Users/azare/Documents/Dev/Cpp/direct3d/vertex-shader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile("C:/Users/azare/Documents/Dev/Cpp/direct3d/pixel-shader.hlsl");
        const Util::ImageAsset diana = Util::LoadImageFile("C:/Users/azare/Documents/Dev/Cpp/direct3d/diana.png");

        meshVertexBuffer = new VertexBuffer(sizeof(Vertex) * cubeVertices.size(), sizeof(Vertex), cubeVertices.data());
        meshIndexBuffer = new IndexBuffer(sizeof(uint32_t) * cubeIndices.size(), sizeof(uint32_t), cubeIndices.data());
        constantBuffer = new ConstantBuffer(sizeof(dx::XMMATRIX));
        shader = new Shader(vertexShaderCode, pixelShaderCode);
        sampler = new Sampler();
        texture = new Texture(diana.Width, diana.Height, diana.Data);

        while (window->IsRunning())
        {
            glfwPollEvents();
            
            static float theta{0};
            static bool dir{true};

            theta += 0.05f * (static_cast<int>(dir) - 0.5f) * 2.0f;

            if (theta >=  M_PI_2) dir = !dir;
            if (theta <= -M_PI_2) dir = !dir;

            const static std::array<float, 4> clearColor1{0.2f, 1.0f, 0.5f, 1.0f};

            Ctx()->ClearRenderTargetView(RenderTargetView(), clearColor1.data());
            Ctx()->ClearDepthStencilView(DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
            DrawTestTriangle1(theta, 0);
            window->Present();
        }
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    delete shader;
    delete constantBuffer;
    delete meshIndexBuffer;
    delete meshVertexBuffer;
    delete sampler;
    delete texture;

    delete d3d;
    delete window;

    return 0;
}