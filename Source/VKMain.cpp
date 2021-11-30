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

        const Util::ModelAsset apricotMesh {Util::LoadModelFile("Assets/Models/GitIgnore/apricot.obj")};
        const Util::ImageAsset apricotTexture {Util::LoadImageFile("Assets/Images/GitIgnore/apricot.png")};

        API::Window window(API::Type::Vulkan, WindowMode::Fullscreen);

        Ptr<VK::FrameManager> frameManager = CreatePtr<VK::FrameManager>(0, 1, 2, 2);

        VK::Renderer renderer(vertexShaderCode, fragmentShaderCode, VK::GetSwapChain().GetNumBuffers());

        renderer.Add(apricotMesh, apricotTexture);

        for (size_t i = 0; i < 3; ++i)
        {
            renderer.Add(characterMesh, characterTexture);
        }

        renderer.GetSpaceObject(0).SetPosition(-5, -5, -15);
        renderer.GetSpaceObject(0).SetScale(2.0f);

        renderer.GetSpaceObject(1).SetPosition(-5, -5, -25);
        renderer.GetSpaceObject(2).SetPosition( 5, -5, -15);
        renderer.GetSpaceObject(3).SetPosition( 5, -5, -25);

        renderer.Record(window.GetSize());

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

            frameManager->AcquireSwapChainImage();

                static float theta {0}; theta += deltaTime * 0.1f;
                for (size_t i = 0; i < renderer.GetNumRenderableEntities(); ++i)
                {
                    VK::SpaceObject& spaceObject = renderer.GetSpaceObject(i);
                    spaceObject.SetRotationY(theta);
                }

                renderer.UpdateUniformBuffers(window.GetAspectRatio());
                renderer.Render(frameManager->GetCurrentFrame(), VK::GetSwapChain().GetCurrentImageIndex());
            
            frameManager->Present();

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