#include "Util/Shaders/SPIRV.h"
#include "Util/Aligned.h"
#include "Util/Assets.h"
#include "API/Window.h"
#include "Util/Path.h"
#include "VK/VK.h"

#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

int main()
{
    try
    {
        const Util::TextAsset vertexShaderCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/VertexShader.vert")};
        const Util::TextAsset fragmentShaderCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/FragmentShader.frag")};

        const Util::ModelAsset characterMesh {Util::LoadModelFile("Assets/Models/CharacterModel.fbx")};
        const Util::ImageAsset characterTexture {Util::LoadImageFile("Assets/Images/CharacterTexture.png")};

        API::Window window(API::Type::Vulkan, WindowMode::Windowed, false, Vec2ui {1024, 768});

        VK::Texture2D depthTexture(window.GetSize(), VK::GetDevice().FindDepthFormat(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

        VK::FrameManager frameManager(0, 1, 2, 2);

        VK::Renderer renderer(vertexShaderCode, fragmentShaderCode, VK::GetSwapChain().GetNumBuffers(), depthTexture);

        for (size_t i = 0; i < 4; ++i)
        {
            renderer.Add(characterMesh, characterTexture);
        }

        renderer.GetSpaceObject(0).SetPosition(-5, -5, -15);
        renderer.GetSpaceObject(1).SetPosition(-5, -5, -25);
        renderer.GetSpaceObject(2).SetPosition( 5, -5, -15);
        renderer.GetSpaceObject(3).SetPosition( 5, -5, -25);

        renderer.Record();

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
                LINE_OUT("Entities: " << renderer.GetNumRenderableEntities());
                timer = 0;
            }

            static float addEntityTimer {0}; addEntityTimer += deltaTime;

            if (addEntityTimer >= 1.0f)
            {
                renderer.Add(characterMesh, characterTexture);
                const Vec3f pos {-2.5f, -5.0f, -80.0f};
                const Vec3f randPos {rand() / static_cast<float>(RAND_MAX) * 80, rand() / static_cast<float>(RAND_MAX) * 40, rand() / static_cast<float>(RAND_MAX) * 50};
                renderer.GetSpaceObject(renderer.GetNumRenderableEntities() - 1).SetPosition(pos + randPos);
                renderer.Record();

                addEntityTimer = 0;
            }

            frameManager.AcquireSwapChainImage();

                static float theta {0}; theta += deltaTime;

                for (size_t i = 0; i < renderer.GetNumRenderableEntities(); ++i)
                {
                    VK::SpaceObject& spaceObject = renderer.GetSpaceObject(i);
                    spaceObject.SetRotationY(theta);
                }

                renderer.UpdateUniformBuffers();
                const VK::Frame& frame {*frameManager.GetCurrentFrame()};
                const VkFence& fence {frame.GetInFlightFence()};
                const VkSemaphore& wait {frame.GetSemaphore(0)};
                const VkSemaphore& signal {frame.GetSemaphore(1)};
                const VK::CommandBuffer& cmd {renderer.GetCommandBuffer(VK::GetSwapChain().GetCurrentImageIndex())};
                vkResetFences(VK::GetDevice().GetVkDevice(), 1, &fence);
                cmd.SubmitToQueue(VK::Queues::graphicsQueue, &wait, &signal, fence);
            
            frameManager.Present();

            window.EndFrame();
        }

        VK::GetDevice().WaitIdle();
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}