#include "Window/Window.h"
#include "Common/Vertex.h"
#include "Util/Assets.h"
#include "API/Buffer.h"
#include "DX/DX.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_glfw.h>

static Ptr<API::VertexBuffer> meshVertexBuffer{nullptr};
static Ptr<API::IndexBuffer> meshIndexBuffer{nullptr};
static Ptr<API::UniformBuffer> constantBuffer{nullptr};
static Ptr<DX::Shader> shader{nullptr};
static Ptr<DX::Sampler> sampler{nullptr};
static Ptr<DX::Texture> texture{nullptr};

void RenderMesh(const float angleX, const float angleY, const unsigned int numIndices)
{
    meshVertexBuffer->Bind(0u);
    meshIndexBuffer->Bind();
    constantBuffer->Bind();
    shader->Bind();
    sampler->Bind();
    texture->Bind();

    const DX::XMMATRIX transform = 
        DX::XMMatrixScaling(1, 1, 1) *
        DX::XMMatrixRotationX(angleX) * 
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

void InitResources(const Util::TextAsset& vsCode, const Util::TextAsset& psCode, const Util::ModelAsset& characterMesh, const Util::ImageAsset& characterTexture)
{
    meshVertexBuffer = API::VertexBuffer::Create(sizeof(Vertex) * characterMesh.vertices.size(), sizeof(Vertex), characterMesh.vertices.data());
    meshIndexBuffer = API::IndexBuffer::Create(sizeof(unsigned int) * characterMesh.indices.size(), sizeof(unsigned int), characterMesh.indices.data());
    constantBuffer = API::UniformBuffer::Create(sizeof(DX::XMMATRIX), 0, nullptr);
    
    shader = CreatePtr<DX::Shader>(vsCode, psCode);
    sampler = CreatePtr<DX::Sampler>();
    texture = CreatePtr<DX::Texture>(characterTexture.width, characterTexture.height, characterTexture.data);
}

int main()
{
    try
    {
        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/VertexShader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/PixelShader.hlsl");
        const Util::ModelAsset characterMesh = Util::LoadModelFile(PROJECT_ROOT_DIR "/Assets/Models/CharacterModel.fbx");
        const Util::ImageAsset characterTexture = Util::LoadImageFile(PROJECT_ROOT_DIR "/Assets/Images/CharacterTexture.png");

        Ptr<Window> window = CreatePtr<Window>();
        Ptr<API::Instance> instance = API::Instance::Create(*window, API::Type::DirectX);

        InitResources(vertexShaderCode, pixelShaderCode, characterMesh, characterTexture);

        
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;
        //io.ConfigViewportsNoDefaultParent = true;
        //io.ConfigDockingAlwaysTabBar = true;
        //io.ConfigDockingTransparentPayload = true;
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        // ImGui_ImplGlfw_Init(window->GetHandle(), true);
        ImGui_ImplGlfw_InitForOther(window->GetHandle(), true);
        ImGui_ImplDX11_Init(DX::GetDevice(), DX::GetContext());

        while (window->IsRunning())
        {
            // Start the Dear ImGui frame
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            window->BeginFrame();

                static float theta{0};
                theta += 0.05f;

                DX::GetRenderTargetView()->Bind();
                DX::GetRenderTargetView()->Clear();
                RenderMesh(0, theta, characterMesh.indices.size());

                ImGui::ShowDemoWindow();
                
                ImGui::Render();
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                }

            window->EndFrame();
        }

        // Cleanup
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}