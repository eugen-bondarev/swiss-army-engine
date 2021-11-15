#include "window/window.h"
#include "dx/buffer.h"
#include "dx/shader.h"
#include "dx/texture.h"
#include "dx/sampler.h"
#include "dx/math.h"

#include "util/assets.h"

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

DX::VertexBuffer*   meshVertexBuffer{nullptr};
DX::IndexBuffer*    meshIndexBuffer{nullptr};
DX::ConstantBuffer* constantBuffer{nullptr};
DX::Shader*         shader{nullptr};
DX::Sampler*        sampler{nullptr};
DX::Texture*        texture{nullptr};

void RenderMesh(const float AngleX, const float AngleY)
{
    shader->Bind();
    meshVertexBuffer->Bind(sizeof(Vertex), 0u);
    meshIndexBuffer->Bind();
    constantBuffer->Bind();
    sampler->Bind();
    texture->Bind();

    DX::XMMATRIX transform = 
        DX::XMMatrixRotationX(AngleX) * 
        DX::XMMatrixRotationY(AngleY) * 
        DX::XMMatrixRotationZ(AngleY) * 
        DX::XMMatrixTranslation(0, 0, 3) * 
        DX::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

    DX::XMMATRIX* data = (DX::XMMATRIX*) constantBuffer->Map();
    *data = DX::XMMatrixTranspose(transform);
    constantBuffer->Unmap();

    DX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(DX::GetContext()->DrawIndexed(cubeIndices.size(), 0u, 0u));
}

int main()
{
    Window* window{nullptr};
    DX::Instance* dxInstance{nullptr};

    try
    {
        window = new Window();
        dxInstance = new DX::Instance(glfwGetWin32Window(window->handle));

        DX::Instance::SetViewport(0u, 0u, 800u, 600u);

        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/assets/shaders/vertex-shader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/assets/shaders/pixel-shader.hlsl");
        const Util::ImageAsset diana = Util::LoadImageFile(PROJECT_ROOT_DIR "/assets/images/diana.png");

        meshVertexBuffer = new DX::VertexBuffer(sizeof(Vertex) * cubeVertices.size(), sizeof(Vertex), cubeVertices.data());
        meshIndexBuffer = new DX::IndexBuffer(sizeof(uint32_t) * cubeIndices.size(), sizeof(uint32_t), cubeIndices.data());
        constantBuffer = new DX::ConstantBuffer(sizeof(DX::XMMATRIX));
        shader = new DX::Shader(vertexShaderCode, pixelShaderCode);
        sampler = new DX::Sampler();
        texture = new DX::Texture(diana.Width, diana.Height, diana.Data);

        while (window->IsRunning())
        {
            glfwPollEvents();
            
            static float theta   {0};
            static bool  dir     {true};
            static float rotSpeed{0.05f};

            theta += rotSpeed * (static_cast<int>(dir) - 0.5f) * 2.0f;

            if (theta >=  M_PI_2) dir = !dir;
            if (theta <= -M_PI_2) dir = !dir;

            const static std::array<float, 4> clearColor{0.2f, 1.0f, 0.5f, 1.0f};

            DX::GetContext()->ClearRenderTargetView(DX::GetRenderTargetView(), clearColor.data());
            DX::GetContext()->ClearDepthStencilView(DX::GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
            RenderMesh(theta, theta);
            window->Present();
        }
    }
    catch (const std::runtime_error& Exception)
    {
        MessageBox(nullptr, Exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    delete shader;
    delete constantBuffer;
    delete meshIndexBuffer;
    delete meshVertexBuffer;
    delete sampler;
    delete texture;

    delete dxInstance;
    delete window;

    return 0;
}