#include "window/window.h"

#include "dx/dx.h"

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

static std::vector<Vertex> quadVertices = 
{
    { { -1,  1, 0 }, { 0, 0 } },
    { {  1,  1, 0 }, { 1, 0 } },
    { {  1, -1, 0 }, { 1, 1 } },
    { { -1, -1, 0 }, { 0, 1 } }
};

static std::vector<uint32_t> quadIndices = 
{
    0, 1, 2,  0, 2, 3
};

static Ptr<DX::VertexBuffer>   meshVertexBuffer{nullptr};
static Ptr<DX::IndexBuffer>    meshIndexBuffer{nullptr};
static Ptr<DX::ConstantBuffer> constantBuffer{nullptr};
static Ptr<DX::Shader>         shader{nullptr};
static Ptr<DX::Sampler>        sampler{nullptr};
static Ptr<DX::Texture>        texture{nullptr};

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

    DX::XMMATRIX* data = static_cast<DX::XMMATRIX*>(constantBuffer->Map());
    *data = DX::XMMatrixTranspose(transform);
    constantBuffer->Unmap();

    DX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(DX::GetContext()->DrawIndexed(quadIndices.size(), 0u, 0u));
}

int main()
{
    Ptr<Window> window;
    Ptr<DX::Instance> dxInstance;

    try
    {
        window = CreatePtr<Window>();
        dxInstance = CreatePtr<DX::Instance>(glfwGetWin32Window(window->handle));

        DX::Instance::SetViewport(0u, 0u, 800u, 600u);

        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/assets/shaders/vertex-shader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/assets/shaders/pixel-shader.hlsl");
        const Util::ImageAsset diana = Util::LoadImageFile(PROJECT_ROOT_DIR "/assets/images/diana.png");

        meshVertexBuffer = CreatePtr<DX::VertexBuffer>(sizeof(Vertex) * quadVertices.size(), sizeof(Vertex), quadVertices.data());
        meshIndexBuffer = CreatePtr<DX::IndexBuffer>(sizeof(uint32_t) * quadIndices.size(), sizeof(uint32_t), quadIndices.data());
        constantBuffer = CreatePtr<DX::ConstantBuffer>(sizeof(DX::XMMATRIX));
        shader = CreatePtr<DX::Shader>(vertexShaderCode, pixelShaderCode);
        sampler = CreatePtr<DX::Sampler>();
        texture = CreatePtr<DX::Texture>(diana.Width, diana.Height, diana.Data);

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

    return 0;
}