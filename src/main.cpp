#include "window/window.h"

#include "dx/dx.h"

#include "common/vertex.h"
#include "util/assets.h"

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
        DX::XMMatrixPerspectiveFovLH(70.0f * M_PI / 180.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);

    DX::XMMATRIX* data = static_cast<DX::XMMATRIX*>(constantBuffer->Map());
    *data = DX::XMMatrixTranspose(transform);
    constantBuffer->Unmap();

    DX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D_TRY(DX::GetContext()->DrawIndexed(NumIndices, 0u, 0u));
}

int main()
{
    Ptr<Window> window;
    Ptr<DX::Instance> dxInstance;

    try
    {
        window = CreatePtr<Window>(1920u, 1080u, WindowMode::Windowed);
        dxInstance = CreatePtr<DX::Instance>(glfwGetWin32Window(window->handle));

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplDX11_Init(DX::GetDevice(), DX::GetContext());
        ImGui_ImplGlfw_InitForOther(window->handle, true);

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

        Ptr<DX::RenderTargetView> renderTarget = CreatePtr<DX::RenderTargetView>(nullptr, true);

        while (window->IsRunning())
        {
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            static float theta   {0};
            static bool  dir     {true};
            static float rotSpeed{0.05f};

            theta += rotSpeed * (static_cast<int>(dir) - 0.5f) * 2.0f;

            if (theta >=  M_PI_2) dir = !dir;
            if (theta <= -M_PI_2) dir = !dir;

            const static std::array<float, 4> clearColor{0.2f, 1.0f, 0.5f, 1.0f};

            renderTarget->Bind();
            renderTarget->Clear(clearColor);
            RenderMesh(theta, theta, characterMesh.Indices.size());
            
            DX::GetRenderTargetView()->Bind();
            DX::GetRenderTargetView()->Clear();
            
            ImGui::Begin("Render target");
                ImGui::Image(renderTarget->GetTexture()->GetDXView(), ImVec2(800, 600));
            ImGui::End();

            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            window->Present(1, 0);
        }
    }
    catch (const std::runtime_error& Exception)
    {
        MessageBox(nullptr, Exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}