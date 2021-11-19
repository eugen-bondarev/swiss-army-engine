#include "Window/Window.h"
#include "Common/Vertex.h"
#include "Util/Assets.h"
#include "DX/DX.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_glfw.h>

#include <assimp/Importer.hpp>

static Ptr<DX::VertexBuffer>   meshVertexBuffer1{nullptr};
static Ptr<DX::IndexBuffer>    meshIndexBuffer1{nullptr};
static Ptr<DX::ConstantBuffer> constantBuffer1{nullptr};
static Ptr<DX::Shader>         shader1{nullptr};
static Ptr<DX::Sampler>        sampler1{nullptr};
static Ptr<DX::Texture>        texture1{nullptr};

static Ptr<DX::VertexBuffer>   meshVertexBuffer2{nullptr};
static Ptr<DX::IndexBuffer>    meshIndexBuffer2{nullptr};
static Ptr<DX::ConstantBuffer> constantBuffer2{nullptr};
static Ptr<DX::Shader>         shader2{nullptr};
static Ptr<DX::Sampler>        sampler2{nullptr};
static Ptr<DX::Texture>        texture2{nullptr};

void RenderMesh1(const float angleX, const float angleY, const unsigned int numIndices)
{
    meshVertexBuffer1->Bind(sizeof(Vertex), 0u);
    meshIndexBuffer1->Bind();
    constantBuffer1->Bind();
    shader1->Bind();
    sampler1->Bind();
    texture1->Bind();

    const DX::XMMATRIX transform = 
        DX::XMMatrixScaling(1, 1, 1) *
        DX::XMMatrixRotationX(angleX) * 
        DX::XMMatrixRotationY(angleY + M_PI) * 
        DX::XMMatrixRotationZ(0) * 
        DX::XMMatrixTranslation(0, -5, 10) * 
        DX::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, static_cast<float>(DX::GetSwapChain()->GetWidth()) / static_cast<float>(DX::GetSwapChain()->GetHeight()), 0.1f, 1000.0f);

    DX::XMMATRIX* data = static_cast<DX::XMMATRIX*>(constantBuffer1->Map());
    *data = DX::XMMatrixTranspose(transform);
    constantBuffer1->Unmap();

    DX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(DX::GetContext()->DrawIndexed(numIndices, 0u, 0u));
}

void RenderMesh2(const float angleX, const float angleY, const unsigned int numIndices)
{
    meshVertexBuffer2->Bind(sizeof(Vertex), 0u);
    meshIndexBuffer2->Bind();
    constantBuffer2->Bind();
    shader2->Bind();
    sampler2->Bind();
    texture2->Bind();

    const DX::XMMATRIX transform = 
        DX::XMMatrixScaling(1, 1, 1) *
        DX::XMMatrixRotationX(angleX) * 
        DX::XMMatrixRotationY(angleY + M_PI) * 
        DX::XMMatrixRotationZ(0) * 
        DX::XMMatrixTranslation(0, -5, 10) * 
        DX::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, static_cast<float>(DX::GetSwapChain()->GetWidth()) / static_cast<float>(DX::GetSwapChain()->GetHeight()), 0.1f, 1000.0f);

    DX::XMMATRIX* data = static_cast<DX::XMMATRIX*>(constantBuffer2->Map());
    *data = DX::XMMatrixTranspose(transform);
    constantBuffer2->Unmap();

    DX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(DX::GetContext()->DrawIndexed(numIndices, 0u, 0u));
}

void InitResources1(const Util::TextAsset& vsCode, const Util::TextAsset& psCode, const Util::ModelAsset& characterMesh, const Util::ImageAsset& characterTexture)
{
    meshVertexBuffer1 = CreatePtr<DX::VertexBuffer>(sizeof(Vertex) * characterMesh.vertices.size(), sizeof(Vertex), characterMesh.vertices.data());
    meshIndexBuffer1 = CreatePtr<DX::IndexBuffer>(sizeof(unsigned int) * characterMesh.indices.size(), sizeof(unsigned int), characterMesh.indices.data());
    constantBuffer1 = CreatePtr<DX::ConstantBuffer>(sizeof(DX::XMMATRIX));
    shader1 = CreatePtr<DX::Shader>(vsCode, psCode);
    sampler1 = CreatePtr<DX::Sampler>();
    texture1 = CreatePtr<DX::Texture>(characterTexture.width, characterTexture.height, characterTexture.data);
}

void InitResources2(const Util::TextAsset& vsCode, const Util::TextAsset& psCode, const Util::ModelAsset& characterMesh, const Util::ImageAsset& characterTexture)
{
    meshVertexBuffer2 = CreatePtr<DX::VertexBuffer>(sizeof(Vertex) * characterMesh.vertices.size(), sizeof(Vertex), characterMesh.vertices.data());
    meshIndexBuffer2 = CreatePtr<DX::IndexBuffer>(sizeof(unsigned int) * characterMesh.indices.size(), sizeof(unsigned int), characterMesh.indices.data());
    constantBuffer2 = CreatePtr<DX::ConstantBuffer>(sizeof(DX::XMMATRIX));
    shader2 = CreatePtr<DX::Shader>(vsCode, psCode);
    sampler2 = CreatePtr<DX::Sampler>();
    texture2 = CreatePtr<DX::Texture>(characterTexture.width, characterTexture.height, characterTexture.data);
}

int main()
{
    try
    {
        Window window1(WindowMode::Windowed, true, 800, 600, "Window 1");
        Window window2(WindowMode::Windowed, true, 800, 600, "Window 2");

        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/VertexShader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/PixelShader.hlsl");
        const Util::ModelAsset characterMesh = Util::LoadModelFile(PROJECT_ROOT_DIR "/Assets/Models/CharacterModel.fbx");
        const Util::ImageAsset characterTexture = Util::LoadImageFile(PROJECT_ROOT_DIR "/Assets/Images/CharacterTexture.png");

        DX::Instance instance1(window1);         
        InitResources1(vertexShaderCode, pixelShaderCode, characterMesh, characterTexture);

        DX::Instance instance2(window2);
        InitResources2(vertexShaderCode, pixelShaderCode, characterMesh, characterTexture);

        while (window1.IsRunning())
        {
            static float theta{0};
            static bool  dir{true};
            static float rotSpeed{0.05f};

            theta += rotSpeed * (static_cast<int>(dir) - 0.5f) * 2.0f;

            if (theta >=  M_PI_2) dir = !dir;
            if (theta <= -M_PI_2) dir = !dir;

            DX::MakeInstanceCurrent(&instance1);

            window1.BeginFrame();
                DX::GetRenderTargetView()->Bind();
                DX::GetRenderTargetView()->Clear();
                RenderMesh1(0, theta, characterMesh.indices.size());            
            window1.EndFrame();

            DX::MakeInstanceCurrent(&instance2);

            window2.BeginFrame();
                DX::GetRenderTargetView()->Bind();
                DX::GetRenderTargetView()->Clear();
                RenderMesh2(theta, theta, characterMesh.indices.size());            
            window2.EndFrame();
        }
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}