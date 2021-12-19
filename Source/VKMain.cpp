#include "Window/Events/Keyboard.h"
#include "Window/Events/Mouse.h"
#include "Util/Shaders/SPIRV.h"
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include "Util/Assets.h"
#include <entt/entt.hpp>
#include "API/Window.h"
#include <imgui.h>
#include "VK/VK.h"

#include <SRM/ResourceManager.h>
#include <SRM/Exception.h>

static const Util::ModelAsset<PredefinedVertexLayouts::Vertex2D> square = {
    { { { -0.5f, -0.5f }, {1, 1} },
      { { -0.5f,  0.5f }, {1, 0} },
      { {  0.5f,  0.5f }, {0, 0} },
      { {  0.5f, -0.5f }, {0, 1} } },
    { 0, 1, 2, 2, 3, 0 }
};

int main()
{
    try
    {
        srm::ResourceManager resourceManager{ 
            Util::Path::projectRoot + "/Assets", 
            "table.asu", 
            "data.asu", 
            srm::ResourceManager::Mode::Default 
        };

        struct
        {
            const Util::TextAsset vsCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/VertexShader.vert")};
            const Util::TextAsset fsCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/FragmentShader.frag")};
        } shader3D;

        struct
        {
            const Util::TextAsset vsCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/GUI/VertexShader.vert")};
            const Util::TextAsset fsCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/GUI/FragmentShader.frag")};
        } shaderGUI;

        //const Util::ModelAsset<PredefinedVertexLayouts::Vertex3D> characterMesh{ Util::LoadModelFile("Assets/Models/CharacterModel.fbx") };
        srm::Resource characterMeshResource;
        srm::Resource characterTextureResource;
        try
        {
            characterMeshResource = resourceManager.Load("Models/CharacterModel.fbx");
            characterTextureResource = resourceManager.Load("Images/CharacterTexture.png");
        }
        catch (const srm::Exception& exception)
        {
            LINE_OUT(exception.what());
        }

        const Util::ModelAsset<PredefinedVertexLayouts::Vertex3D> characterMesh{ Util::LoadModelFile(characterMeshResource) };
        const Util::ImageAsset characterTexture{ Util::LoadImageFile(characterTextureResource) };

        //const Util::ImageAsset characterTexture {Util::LoadImageFile("Assets/Images/CharacterTexture.png")};

        API::Window window(API::Type::Vulkan, WindowMode::Windowed, false, Vec2ui(1024, 768));

        VK::RenderSequence sequence;

        VK::Renderer3D& renderer3D = sequence.Emplace<VK::Renderer3D>(
            shader3D.vsCode,
            shader3D.fsCode,
            8,
            RendererFlags_UseDepth | RendererFlags_Clear | RendererFlags_Offscreen
        );

        const size_t foo{ 254 };

        VK::RendererGUI& rendererGUI = sequence.Emplace<VK::RendererGUI>(
            shaderGUI.vsCode,
            shaderGUI.fsCode,
            0,
            RendererFlags_None
        );

        VK::RendererImGui& rendererImGui = sequence.Emplace<VK::RendererImGui>(RendererFlags_Load | RendererFlags_Output);
        
        sequence.InitFrames();

        for (size_t i = 0; i < 2; ++i)
        {
            renderer3D.Add(characterMesh, characterTexture);
        }

        renderer3D.GetSpaceObject(0).SetPosition(-5, -5, -15);
        renderer3D.GetSpaceObject(1).SetPosition(-5, -5, -25);

        rendererGUI.Add(square, characterTexture);
        // rendererGUI.GetSpaceObject(0).SetScale(-Math::CastTo<Vec2f>(window.GetSize()), 1.0f);
        rendererGUI.GetSpaceObject(0).SetScale(-512.0f, -512.0f, 1.0f);
        rendererGUI.GetSpaceObject(0).SetPosition(0.0f, 0.0f, 0.0f);
        rendererGUI.GetOrthogonalSpace().Set(-512.0f, 512.0f, -384.0f, 384.0f, 1.0f);
        rendererGUI.renderable[0]->GetDescriptorSet().SetBinding(2, renderer3D.output.imageView[0]->GetVkDescriptor());

        window.ResizeSubscribe([&](const Vec2ui newSize)
        {
            rendererGUI.renderable[0]->GetDescriptorSet().SetBinding(2, renderer3D.output.imageView[0]->GetVkDescriptor());
            rendererGUI.RecordAll();
        });
        
        renderer3D.RecordAll();
        rendererGUI.RecordAll();

        while (window.IsRunning())
        {
            window.BeginFrame();

            const float speed {window.GetDeltaTime() * 10.0f};

            if (!ImGui::GetIO().WantCaptureMouse)
            {
                if (window.GetKeyboard().KeyDown(GLFW_KEY_ESCAPE))
                {
                    window.Close();
                }
                if (window.GetMouse().ButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                {
                    renderer3D.GetPerspectiveSpace().camera.rotation.y += window.GetMouse().GetDeltaPosition().x * 0.07f;
                    renderer3D.GetPerspectiveSpace().camera.rotation.x += window.GetMouse().GetDeltaPosition().y * 0.07f;
                }
                if (window.GetKeyboard().KeyDown(GLFW_KEY_W))
                {
                    renderer3D.GetPerspectiveSpace().camera.position += renderer3D.GetPerspectiveSpace().forwardVector * window.GetDeltaTime() * 10.0f;
                }
                if (window.GetKeyboard().KeyDown(GLFW_KEY_S))
                {
                    renderer3D.GetPerspectiveSpace().camera.position -= renderer3D.GetPerspectiveSpace().forwardVector * window.GetDeltaTime() * 10.0f;
                }
                if (window.GetKeyboard().KeyDown(GLFW_KEY_D))
                {
                    renderer3D.GetPerspectiveSpace().camera.position += renderer3D.GetPerspectiveSpace().rightVector * window.GetDeltaTime() * 10.0f;
                }
                if (window.GetKeyboard().KeyDown(GLFW_KEY_A))
                {
                    renderer3D.GetPerspectiveSpace().camera.position -= renderer3D.GetPerspectiveSpace().rightVector * window.GetDeltaTime() * 10.0f;
                }
                if (window.GetKeyboard().KeyDown(GLFW_KEY_SPACE))
                {
                    renderer3D.GetPerspectiveSpace().camera.position.y += speed;
                }
                if (window.GetKeyboard().KeyDown(GLFW_KEY_LEFT_SHIFT))
                {
                    renderer3D.GetPerspectiveSpace().camera.position.y -= speed;
                }
                renderer3D.GetPerspectiveSpace().UpdateProjectionMatrix();
            }

            static bool show {true};
            if (show)
            {
                ImGui::Begin("Foo");
                ImGui::End();
            }

            ImGui::Begin("Bar");
                ImGui::Checkbox("Show foo?", &show);
            ImGui::End();

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

            renderer3D.UpdateUniformBuffers();
            rendererGUI.UpdateUniformBuffers();

            static float theta {0}; theta += deltaTime * 0.5f;
            for (size_t i = 0; i < renderer3D.GetNumRenderableEntities(); ++i)
            {
                VK::SpaceObject& spaceObject = renderer3D.GetSpaceObject(i);
                spaceObject.SetRotationY(theta);
            }

            sequence.Render();

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