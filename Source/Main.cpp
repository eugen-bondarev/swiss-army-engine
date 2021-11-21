#include "Util/Assets.h"
#include "API/Window.h"
#include "VK/vk.h"

int main()
{
    try
    {
        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/VertexShader.hlsl");
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/PixelShader.hlsl");
        const Util::ModelAsset characterMesh = Util::LoadModelFile(PROJECT_ROOT_DIR "/Assets/Models/CharacterModel.fbx");
        const Util::ImageAsset characterTexture = Util::LoadImageFile(PROJECT_ROOT_DIR "/Assets/Images/CharacterTexture.png");

        Ptr<API::Window> window = CreatePtr<API::Window>(API::Type::Vulkan, WindowMode::Windowed, true, 800, 600);

        Engine::Vk::Bootstrap(window->GetHandle());

        while (window->IsRunning())
        {
            window->BeginFrame();
        }

        Engine::Vk::Shutdown();
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}