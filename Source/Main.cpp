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

void RenderMesh(const float AngleX, const float AngleY, const unsigned int NumIndices)
{
    shader->Bind();
    meshVertexBuffer->Bind(sizeof(Vertex), 0u);
    meshIndexBuffer->Bind();
    constantBuffer->Bind();
    sampler->Bind();
    texture->Bind();

    DX::XMMATRIX transform = 
        DX::XMMatrixScaling(1, 1, 1) *
        DX::XMMatrixRotationX(0) * 
        DX::XMMatrixRotationY(AngleY + M_PI) * 
        DX::XMMatrixRotationZ(0) * 
        DX::XMMatrixTranslation(0, -5, 10) * 
        DX::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, static_cast<float>(DX::GetSwapChain()->GetWidth()) / static_cast<float>(DX::GetSwapChain()->GetHeight()), 0.1f, 1000.0f);

    DX::XMMATRIX* data = static_cast<DX::XMMATRIX*>(constantBuffer->Map());
    *data = DX::XMMatrixTranspose(transform);
    constantBuffer->Unmap();

    DX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(DX::GetContext()->DrawIndexed(NumIndices, 0u, 0u));
}

static void InitImGui(GLFWwindow* Handle)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplDX11_Init(DX::GetDevice(), DX::GetContext());
    ImGui_ImplGlfw_InitForOther(Handle, true);
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
        Window window(1920u, 1080u, WindowMode::Windowed);
        DX::Instance instance(&window);

        InitImGui(window.GetHandle());

        DX::Instance::SetViewport(0u, 0u, 1920u, 1080u);

        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/assets/shaders/vertex-shader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/assets/shaders/pixel-shader.hlsl");
        const Util::ImageAsset diana = Util::LoadImageFile(PROJECT_ROOT_DIR "/assets/images/1.png");
        const Util::ModelAsset characterMesh = Util::LoadModelFile(PROJECT_ROOT_DIR "/assets/models/1.fbx");

        meshVertexBuffer = CreatePtr<DX::VertexBuffer>(sizeof(Vertex) * characterMesh.Vertices.size(), sizeof(Vertex), characterMesh.Vertices.data());
        meshIndexBuffer = CreatePtr<DX::IndexBuffer>(sizeof(unsigned int) * characterMesh.Indices.size(), sizeof(unsigned int), characterMesh.Indices.data());
        constantBuffer = CreatePtr<DX::ConstantBuffer>(sizeof(DX::XMMATRIX));
        shader = CreatePtr<DX::Shader>(vertexShaderCode, pixelShaderCode);
        sampler = CreatePtr<DX::Sampler>();
        texture = CreatePtr<DX::Texture>(diana.Width, diana.Height, diana.Data);

        Ptr<DX::RenderTargetView> offscreenPass = CreatePtr<DX::RenderTargetView>(1920u, 1080u, true);

        window.ResizeCallbacks.emplace_back([&](int Width, int Height)
        {
            offscreenPass.reset();
            offscreenPass = CreatePtr<DX::RenderTargetView>(Width, Height, true);
        });

        while (window.IsRunning())
        {
            glfwPollEvents();
            
            static float theta   {0};

            {
                static bool  dir     {true};
                static float rotSpeed{0.05f};

                theta += rotSpeed * (static_cast<int>(dir) - 0.5f) * 2.0f;

                if (theta >=  M_PI_2) dir = !dir;
                if (theta <= -M_PI_2) dir = !dir;
            }

            offscreenPass->Bind();
            offscreenPass->Clear({0.2f, 1.0f, 0.5f, 1.0f});
            RenderMesh(theta, theta, characterMesh.Indices.size());
            
            BeginImGuiFrame();

            ImGui::Begin("Render target");
                ImGui::Image(offscreenPass->GetTexture()->GetDXView(), ImVec2(800, 450));
            ImGui::End();
            
            DX::GetRenderTargetView()->Bind();
            DX::GetRenderTargetView()->Clear();

            EndImGuiFrame();

            DX::GetSwapChain()->Present();
        }

        ShutdownImGui();
    }
    catch (const std::runtime_error& Exception)
    {
        MessageBox(nullptr, Exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}