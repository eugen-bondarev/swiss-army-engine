#include "Util/Shaders/SPIRV.h"
#include "Util/Aligned.h"
#include "Util/Assets.h"
#include "API/Window.h"
#include "Util/Path.h"
#include "VK/VK.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

void ImGuiInit(
    GLFWwindow* handle, 
    const VkInstance& vkInstance, 
    const VkPhysicalDevice& vkPhysicalDevice,
    const VkDevice& vkDevice,
    const uint32_t queueFamily,
    const VkQueue& queue,
    const VK::DescriptorPool& descriptorPool,
    const VK::Pipeline& pipeline
)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(handle, true);
    ImGui_ImplVulkan_InitInfo initInfo{};
    initInfo.Instance = vkInstance;
    initInfo.PhysicalDevice = vkPhysicalDevice;
    initInfo.Device = vkDevice;
    initInfo.QueueFamily = queueFamily;
    initInfo.Queue = queue;
    initInfo.PipelineCache = nullptr;
    initInfo.DescriptorPool = descriptorPool.GetVkDescriptorPool();
    initInfo.Allocator = nullptr;
    initInfo.MinImageCount = 3;
    initInfo.ImageCount = 3;
    initInfo.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&initInfo, pipeline.GetRenderPass().GetVkRenderPass());

    const VK::CommandPool pool;
    const VK::CommandBuffer cmd(pool);

    cmd.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        ImGui_ImplVulkan_CreateFontsTexture(cmd.GetVkCommandBuffer());
    cmd.End();

    cmd.SubmitToQueue(queue);

    vkDeviceWaitIdle(vkDevice);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void ImGuiShutdown()
{    
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main()
{
    try
    {
        const Util::TextAsset vertexShaderCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/VertexShader.vert")};
        const Util::TextAsset fragmentShaderCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/FragmentShader.frag")};

        const Util::ModelAsset characterMesh {Util::LoadModelFile("Assets/Models/CharacterModel.fbx")};
        const Util::ImageAsset characterTexture {Util::LoadImageFile("Assets/Images/CharacterTexture.png")};

        API::Window window(API::Type::Vulkan, WindowMode::Windowed, false, Vec2ui {1024, 786});

        Ptr<VK::FrameManager> frameManager = CreatePtr<VK::FrameManager>(0, 2, 3, 3);

        VK::Renderer3D renderer(vertexShaderCode, fragmentShaderCode, VK::GetSwapChain().GetNumBuffers(), 0, true, false);
        VK::RendererGUI imGuiRenderer(vertexShaderCode, fragmentShaderCode, VK::GetSwapChain().GetNumBuffers(), 0, false, true);

        ImGuiInit(
            window.GetHandle(),
            VK::GetInstance().GetVkInstance(),
            VK::GetDevice().GetVkPhysicalDevice(),
            VK::GetDevice().GetVkDevice(),
            VK::Queues::indices.graphicsFamily.value(),
            VK::Queues::graphicsQueue,
            VK::GetDefaultDescriptorPool(),
            imGuiRenderer.GetPipeline()
        );

        for (size_t i = 0; i < 4; ++i)
        {
            renderer.Add(characterMesh, characterTexture);
        }

        renderer.GetSpaceObject(0).SetPosition(-5, -5, -15);
        renderer.GetSpaceObject(1).SetPosition(-5, -5, -25);
        renderer.GetSpaceObject(2).SetPosition( 5, -5, -15);
        renderer.GetSpaceObject(3).SetPosition( 5, -5, -25);

        // renderer.Record(window.GetSize(), [&](const VkCommandBuffer&) {});
        renderer.RecordAll();

        Vec<VK::Renderer*> renderers { &renderer, &imGuiRenderer };

        while (window.IsRunning())
        {
            window.BeginFrame();

            const float deltaTime {window.GetDeltaTime()};
            static float timer {0}; timer += deltaTime;
            static unsigned int fpsCounter {0};
            static unsigned int numFpsRecordings {0};
            static unsigned int averageFps {0};

            if (timer >= 1.0f)
            {
                fpsCounter += 1.0f / deltaTime;
                numFpsRecordings += 1;
                averageFps = static_cast<unsigned int>(static_cast<float>(fpsCounter) / static_cast<float>(numFpsRecordings));
                VAR_OUT(averageFps);
                timer = 0;
            }

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
                ImGui::ShowDemoWindow();
            ImGui::Render();

            renderer.UpdateUniformBuffers(window.GetAspectRatio());

            frameManager->AcquireSwapChainImage();

                imGuiRenderer.Record(VK::GetSwapChain().GetCurrentImageIndex());
                // imGuiRenderer.Record(window.GetSize(), VK::GetSwapChain().GetCurrentImageIndex(), [&](const VkCommandBuffer& cmd)
                // {
                //     ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
                // });

                static float theta {0}; theta += deltaTime * 0.5f;
                for (size_t i = 0; i < renderer.GetNumRenderableEntities(); ++i)
                {
                    VK::SpaceObject& spaceObject = renderer.GetSpaceObject(i);
                    spaceObject.SetRotationY(theta);
                }

                // for (size_t i = 0; i < renderers.size(); ++i)
                // {
                //     const size_t firstSemaphoreIndex {i};
                //     const size_t lastSemaphoreIndex {firstSemaphoreIndex + 1};
                //     const bool lastRenderCall {lastSemaphoreIndex == renderers.size()};
                //     renderers[i]->Render(
                //         frameManager->GetCurrentFrame(), 
                //         VK::GetSwapChain().GetCurrentImageIndex(), 
                //         lastRenderCall, 
                //         firstSemaphoreIndex, 
                //         lastSemaphoreIndex
                //     );
                // }
                
                renderer.Render(frameManager->GetCurrentFrame(), VK::GetSwapChain().GetCurrentImageIndex(), false, 0, 1);
                imGuiRenderer.Render(frameManager->GetCurrentFrame(), VK::GetSwapChain().GetCurrentImageIndex(), true, 1, 2);
            
            frameManager->Present();

            // Update and Render additional Platform Windows		
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }

            window.EndFrame();
        }

        VK::GetDevice().WaitIdle();
        ImGuiShutdown();
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}