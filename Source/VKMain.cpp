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
    const VK::RenderPass& renderPass
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
    ImGui_ImplVulkan_Init(&initInfo, renderPass.GetVkRenderPass());

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

        API::Window window(API::Type::Vulkan, WindowMode::Windowed, false, Vec2ui(1024, 768));

        Ptr<VK::FrameManager> frameManager = CreatePtr<VK::FrameManager>(0, 3, 4, 3);

        VK::Renderer3D renderer0(
            vertexShaderCode,
            fragmentShaderCode,
            VK::GetSwapChain().GetNumBuffers(),
            0,
            RendererFlags_UseDepth
        );

        VK::Renderer3D renderer1(
            vertexShaderCode,
            fragmentShaderCode,
            VK::GetSwapChain().GetNumBuffers(),
            0,
            RendererFlags_Load | RendererFlags_UseDepth
        );

        VK::RendererGUI imGuiRenderer(
            VK::GetSwapChain().GetNumBuffers(),
            0,
            RendererFlags_Load | RendererFlags_Output
        );

        ImGuiInit(
            window.GetHandle(),
            VK::GetInstance().GetVkInstance(),
            VK::GetDevice().GetVkPhysicalDevice(),
            VK::GetDevice().GetVkDevice(),
            VK::Queues::indices.graphicsFamily.value(),
            VK::Queues::graphicsQueue,
            VK::GetDefaultDescriptorPool(),
            imGuiRenderer.GetRenderPass()
        );

        for (size_t i = 0; i < 2; ++i)
        {
            renderer0.Add(characterMesh, characterTexture);
            renderer1.Add(characterMesh, characterTexture);
        }

        renderer0.GetSpaceObject(0).SetPosition(-5, -5, -15);
        renderer0.GetSpaceObject(1).SetPosition(-5, -5, -25);
        renderer1.GetSpaceObject(0).SetPosition( 5, -5, -15);
        renderer1.GetSpaceObject(1).SetPosition( 5, -5, -25);
        
        renderer0.RecordAll();
        renderer1.RecordAll();

        while (window.IsRunning())
        {
            window.BeginFrame();

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
                ImGui::Begin("Scene");
                {
                    static float* distance = renderer0.GetOrthogonalSpace().GetDistancePtr();
                    if (ImGui::DragFloat("Distance", distance, 0.001f, -128.0f, 128.0f))
                    {
                        renderer0.GetOrthogonalSpace().UpdateProjectionMatrix();
                        renderer1.GetOrthogonalSpace().UpdateProjectionMatrix();
                    }
                }
                {
                    if (ImGui::Button("Switch space"))
                    {
                        static bool currentSpace {true};
                        if (currentSpace)
                        {
                            renderer0.GetPerspectiveSpace().SetAspectRatio(window.GetAspectRatio());
                            renderer1.GetPerspectiveSpace().SetAspectRatio(window.GetAspectRatio());
                        }
                        else
                        {
                            renderer0.GetOrthogonalSpace().UpdateProjectionMatrix();
                            renderer1.GetOrthogonalSpace().UpdateProjectionMatrix();
                        }
                        currentSpace = !currentSpace;
                    }
                }
                ImGui::End();
            ImGui::Render();

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

            renderer0.UpdateUniformBuffers(window.GetAspectRatio());
            renderer1.UpdateUniformBuffers(window.GetAspectRatio());

            frameManager->AcquireSwapChainImage();

                imGuiRenderer.Record(VK::GetSwapChain().GetCurrentImageIndex());

                static float theta {0}; theta += deltaTime * 0.5f;
                for (size_t i = 0; i < renderer0.GetNumRenderableEntities(); ++i)
                {
                    VK::SpaceObject& spaceObject = renderer0.GetSpaceObject(i);
                    spaceObject.SetRotationY(theta);
                }
                
                renderer0.Render(frameManager->GetCurrentFrame(), VK::GetSwapChain().GetCurrentImageIndex(), false, 0, 1);
                renderer1.Render(frameManager->GetCurrentFrame(), VK::GetSwapChain().GetCurrentImageIndex(), false, 1, 2);
                imGuiRenderer.Render(frameManager->GetCurrentFrame(), VK::GetSwapChain().GetCurrentImageIndex(), true, 2, 3);
            
            frameManager->Present();

            window.EndFrame();

            // break;
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