#include "Window/Window.h"
#include "Common/Vertex.h"
#include "Util/Assets.h"
#include "DX/DX.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_glfw.h>

#include <assimp/Importer.hpp>

static Ptr<DX::VertexBuffer>   meshVertexBuffer{nullptr};
static Ptr<DX::IndexBuffer>    meshIndexBuffer{nullptr};
static Ptr<DX::ConstantBuffer> constantBuffer{nullptr};
static Ptr<DX::Shader>         shader{nullptr};
static Ptr<DX::Sampler>        sampler{nullptr};
static Ptr<DX::Texture>        texture{nullptr};

void RenderMesh(const float angleX, const float angleY, const unsigned int numIndices)
{
    const DX::XMMATRIX transform = 
        DX::XMMatrixScaling(1, 1, 1) *
        DX::XMMatrixRotationX(0) * 
        DX::XMMatrixRotationY(angleY + M_PI) * 
        DX::XMMatrixRotationZ(0) * 
        DX::XMMatrixTranslation(0, -5, 10) * 
        DX::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, static_cast<float>(DX::GetSwapChain()->GetWidth()) / static_cast<float>(DX::GetSwapChain()->GetHeight()), 0.1f, 1000.0f);

    DX::XMMATRIX* data = static_cast<DX::XMMATRIX*>(constantBuffer->Map());
    *data = DX::XMMatrixTranspose(transform);
    constantBuffer->Unmap();

    DX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(DX::GetContext()->DrawIndexed(numIndices, 0u, 0u));
}

static void InitImGui(GLFWwindow* handle)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplDX11_Init(DX::GetDevice(), DX::GetContext());
    ImGui_ImplGlfw_InitForOther(handle, true);
}

static void ShutdownImGui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

static void BeginImGuiFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

static void EndImGuiFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
    try
    {
        Window window;
        DX::Instance instance(window);

        InitImGui(window.GetHandle());

        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/VertexShader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/PixelShader.hlsl");
        const Util::ImageAsset diana = Util::LoadImageFile(PROJECT_ROOT_DIR "/Assets/Images/CharacterTexture.png");
        const Util::ModelAsset characterMesh = Util::LoadModelFile(PROJECT_ROOT_DIR "/Assets/Models/CharacterModel.fbx");

        meshVertexBuffer = CreatePtr<DX::VertexBuffer>(sizeof(Vertex) * characterMesh.vertices.size(), sizeof(Vertex), characterMesh.vertices.data());
        meshVertexBuffer->Bind(sizeof(Vertex), 0u);

        meshIndexBuffer = CreatePtr<DX::IndexBuffer>(sizeof(unsigned int) * characterMesh.indices.size(), sizeof(unsigned int), characterMesh.indices.data());
        meshIndexBuffer->Bind();

        constantBuffer = CreatePtr<DX::ConstantBuffer>(sizeof(DX::XMMATRIX));
        constantBuffer->Bind();

        shader = CreatePtr<DX::Shader>(vertexShaderCode, pixelShaderCode);
        shader->Bind();

        sampler = CreatePtr<DX::Sampler>();
        sampler->Bind();

        texture = CreatePtr<DX::Texture>(diana.width, diana.height, diana.data);
        texture->Bind();

        while (window.IsRunning())
        {
            window.BeginFrame();

            static float theta{0};

            {
                static bool  dir{true};
                static float rotSpeed{0.05f};

                theta += rotSpeed * (static_cast<int>(dir) - 0.5f) * 2.0f;

                if (theta >=  M_PI_2) dir = !dir;
                if (theta <= -M_PI_2) dir = !dir;
            }

            DX::GetRenderTargetView()->Bind();
            DX::GetRenderTargetView()->Clear();
            RenderMesh(theta, theta, characterMesh.indices.size());
            
            BeginImGuiFrame();
            
            ImGui::ShowDemoWindow();

            EndImGuiFrame();

            window.EndFrame();
        }

        ShutdownImGui();
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}