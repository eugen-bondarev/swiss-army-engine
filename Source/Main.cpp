#include "Window/RawWindow.h"
#include "Common/Vertex.h"
#include "Util/Assets.h"
#include "API/Sampler.h"
#include "API/Window.h"
#include "API/Buffer.h"
#include "API/Shader.h"

#include "VK/GraphicsContext.h"

#include "DX/DX.h"

#include "VK/Entities/Buffer/Buffer.h"
#include "VK/Entities/Frames/Frame.h"
#include "VK/Entities/Device/QueueFamily.h"

static Ptr<VK::VertexBuffer> meshVertexBuffer{nullptr};
static Ptr<VK::IndexBuffer> meshIndexBuffer{nullptr};

// static Ptr<API::VertexBuffer> meshVertexBuffer{nullptr};
// static Ptr<API::IndexBuffer> meshIndexBuffer{nullptr};
static Ptr<API::UniformBuffer> constantBuffer{nullptr};
static Ptr<API::Shader> shader{nullptr};
static Ptr<API::Sampler> sampler{nullptr};
static Ptr<API::Texture> texture{nullptr};

void RenderMesh(const float angleX, const float angleY, const unsigned int numIndices)
{
    // meshVertexBuffer->Bind(0u);
    // meshIndexBuffer->Bind();
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
    meshVertexBuffer = CreatePtr<VK::VertexBuffer>(sizeof(Vertex) * characterMesh.vertices.size(), sizeof(Vertex), characterMesh.vertices.data());
    meshIndexBuffer = CreatePtr<VK::IndexBuffer>(sizeof(unsigned int) * characterMesh.indices.size(), sizeof(unsigned int), characterMesh.indices.data());

    // meshVertexBuffer = API::VertexBuffer::Create(sizeof(Vertex) * characterMesh.vertices.size(), sizeof(Vertex), characterMesh.vertices.data());
    // meshIndexBuffer = API::IndexBuffer::Create(sizeof(unsigned int) * characterMesh.indices.size(), sizeof(unsigned int), characterMesh.indices.data());
    // constantBuffer = API::UniformBuffer::Create(sizeof(DX::XMMATRIX), 0, nullptr);
    // shader = API::Shader::Create(vsCode, psCode);
    // sampler = API::Sampler::Create();
    // texture = API::Texture::Create(characterTexture.width, characterTexture.height, characterTexture.data);
}

int main()
{
    try
    {
        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/VertexShader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/PixelShader.hlsl");
        const Util::ModelAsset characterMesh = Util::LoadModelFile(PROJECT_ROOT_DIR "/Assets/Models/CharacterModel.fbx");
        const Util::ImageAsset characterTexture = Util::LoadImageFile(PROJECT_ROOT_DIR "/Assets/Images/CharacterTexture.png");

        Ptr<API::Window> window = CreatePtr<API::Window>(API::Type::Vulkan, WindowMode::Windowed, true, 800, 600);

        InitResources(vertexShaderCode, pixelShaderCode, characterMesh, characterTexture);

        VK::FrameManager frameManager(0, 1, 2, 2);

        VK::GraphicsContext* vk = dynamic_cast<VK::GraphicsContext*>(API::GetCurrentGraphicsContext());
        vk->swapChain->InitFramebuffers(vk->pipeline->GetRenderPass()->GetVkRenderPass());

        while (window->IsRunning())
        {
            window->BeginFrame();

            frameManager.AcquireSwapChainImage();
            
            // vkAcquireNextImageKHR(VK::GetDevice()->GetVkDevice(), VK::GetSwapChain()->GetVkSwapChain(), UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &VK::GetSwapChain()->imageIndex);
            //     static float theta{0}; theta += 0.05f;
            //     DX::GetRenderTargetView()->Bind();
            //     DX::GetRenderTargetView()->Clear();
            //     RenderMesh(0, theta, characterMesh.indices.size());
            // window->EndFrame();
            VkSemaphore* wait = &frameManager.GetCurrentFrame()->GetSemaphore(0);
            VkSemaphore* signal = &frameManager.GetCurrentFrame()->GetSemaphore(1);
	        VkFence fence = frameManager.GetCurrentFrame()->GetInFlightFence();

            VK::CommandBuffer* cmd = VK::GetCommandBuffer();
            VK::CommandPool* pool = VK::GetCommandPool();

            pool->Reset();
                cmd->Begin();
                    cmd->BeginRenderPass(vk->pipeline->GetRenderPass(), vk->swapChain->GetCurrentScreenFramebuffer());                                
			            cmd->BindPipeline(vk->pipeline.get());
                            cmd->BindIndexBuffer(meshIndexBuffer.get());
                            cmd->BindDescriptorSets(vk->pipeline.get(), 1, &vk->descriptorSet->GetVkDescriptorSet());
                                cmd->BindVertexBuffers({meshVertexBuffer.get()}, {0});
                                    // cmd->DrawIndexed(characterMesh.indices.size(), 1, 0, 0, 0);
                                    vkCmdDrawIndexed(cmd->GetVkCommandBuffer(), characterMesh.indices.size(), 1, 0, 0, 0);
                    cmd->EndRenderPass();
                cmd->End();

	        vkResetFences(VK::GetDevice()->GetVkDevice(), 1, &fence);
            VK::GetCommandBuffer()->SubmitToQueue(VK::Queues::graphicsQueue, wait, signal, fence);

            frameManager.Present();

            // throw std::runtime_error("");
        }
        VK::GetDevice()->WaitIdle();
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}