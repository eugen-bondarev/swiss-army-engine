#include "VK/Logic/Scene/SpaceObject.h"
#include "Util/Shaders/SPIRV.h"
#include "Util/Aligned.h"
#include "Util/Assets.h"
#include "API/Window.h"
#include "Util/Path.h"
#include "VK/VK.h"

#include <gtc/matrix_transform.hpp>
#include <glm.hpp>


struct PerSceneUBO
{
    glm::mat4x4 proj;
};

// struct PerEntityUBO
// {
//     glm::mat4x4 model;
// };

static Ptr<VK::SceneUniformBuffer<PerSceneUBO>> sceneUniformBuffer;
static Ptr<VK::EntityUniformBuffer<VK::EntityUBO>> entityUniformBuffer;

struct Mesh
{
    Mesh(const Util::ModelAsset& modelAsset, const Util::ImageAsset& imageAsset, const VK::DescriptorSetLayout& descriptorSetLayout)
    {
        vertexBuffer = CreatePtr<VK::VertexBuffer>(modelAsset.vertices);
        indexBuffer = CreatePtr<VK::IndexBuffer>(modelAsset.indices);

        texture = CreatePtr<VK::Texture2D>(imageAsset.size, 4, imageAsset.data);
        
        numIndices = modelAsset.indices.size();

        descriptorSet = CreatePtr<VK::DescriptorSet>(
            VK::GetDefaultDescriptorPool(),
            std::vector<VkDescriptorSetLayout> {descriptorSetLayout.GetVkDescriptorSetLayout()}
        );

		descriptorSet->Update({
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &sceneUniformBuffer->GetVkDescriptor()),
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &entityUniformBuffer->GetVkDescriptor()),
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &texture->GetImageView().GetVkDescriptor())
		});
    }

    Ptr<VK::DescriptorSet> descriptorSet;
    Ptr<VK::VertexBuffer> vertexBuffer;
    Ptr<VK::IndexBuffer> indexBuffer;
    Ptr<VK::Texture2D> texture;
    unsigned int numIndices;
};

constexpr unsigned int numInstances {4};

int main()
{
    try
    {
        const Util::TextAsset vertexShaderCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/VertexShader.vert")};
        const Util::TextAsset fragmentShaderCode {Util::SPIRV::CompileAndExtract("Assets/Shaders/FragmentShader.frag")};

        const Util::ModelAsset characterMesh {Util::LoadModelFile("Assets/Models/CharacterModel.fbx")};
        const Util::ImageAsset characterTexture {Util::LoadImageFile("Assets/Images/CharacterTexture.png")};

        Ptr<API::Window> window = CreatePtr<API::Window>(API::Type::Vulkan, WindowMode::Windowed, false, Vec2ui {1024, 768});

        VK::Image depthImage(window->GetSize(), VK::GetDevice().FindDepthFormat(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        VK::ImageView depthImageView(depthImage, depthImage.GetVkFormat(), VK_IMAGE_ASPECT_DEPTH_BIT);

        VK::FrameManager frameManager(0, 1, 2, 2);

        std::vector<Ptr<VK::CommandPool>> commandPools;
        std::vector<Ptr<VK::CommandBuffer>> commandBuffers;

        for (size_t i = 0; i < VK::GetSwapChain().GetImageViews().size(); ++i)
        {
            Ptr<VK::CommandPool> pool = CreatePtr<VK::CommandPool>();
            commandBuffers.push_back(CreatePtr<VK::CommandBuffer>(*pool));
            commandPools.push_back(std::move(pool));
        }

        const std::vector<VkDescriptorSetLayoutBinding> bindings {{
            VK::CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
            VK::CreateBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC),
            VK::CreateBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        }};

        const VK::DescriptorSetLayout descriptorSetLayout {bindings};
		const VK::BindingDescriptions bindingDescriptors {VK::Vertex::GetBindingDescriptions()};
		const VK::AttributeDescriptions attributeDescriptors {VK::Vertex::GetAttributeDescriptions()};

        VK::Pipeline pipeline {
            vertexShaderCode, 
            fragmentShaderCode,
            window->GetSize(),
            { VK::GetSwapChain().GetDefaultAttachmentDescription(), VK::Util::CreateDefaultDepthAttachment(depthImage) },
            bindingDescriptors,
            attributeDescriptors,
            { descriptorSetLayout.GetVkDescriptorSetLayout() }
        };

	    VK::GetSwapChain().InitFramebuffers(pipeline.GetRenderPass(), depthImageView);

        sceneUniformBuffer = CreatePtr<VK::SceneUniformBuffer<PerSceneUBO>>();
        entityUniformBuffer = CreatePtr<VK::EntityUniformBuffer<VK::EntityUBO>>(numInstances);
        // VK::SceneUniformBuffer<PerSceneUBO> sceneUniformBuffer;
        // VK::EntityUniformBuffer<PerEntityUBO> entityUniformBuffer {numInstances};

        std::vector<VK::SpaceObject> spaceObjects;

        for (size_t i = 0; i < 4; ++i)
        {
            spaceObjects.emplace_back(&(*entityUniformBuffer)()[i]);
        }

        // std::vector<Ptr<Entity>> entities;
        // for (size_t i = 0; i < 4; ++i)
        // {
        //     Ptr<Entity> entity {CreatePtr<Entity>()};
        //     entities.push_back(std::move(entity));
        // }

        std::vector<Mesh> meshes;
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout);
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout);
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout);
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout);

        // for (size_t i = 0; i < 4; ++i)
        // {
        //     Entity& entity {*entities[i]};
        //     Transform& transform {*entity.AddComponent<Transform>()};
        //     transform.Init(&(*entityUniformBuffer)()[i]);
        // }

        spaceObjects[0].SetPosition(-5, -5, -15);
        spaceObjects[1].SetPosition(-5, -5, -25);
        spaceObjects[2].SetPosition( 5, -5, -15);
        spaceObjects[3].SetPosition( 5, -5, -25);

        for (size_t i = 0; i < commandBuffers.size(); ++i)
        {
            VK::CommandPool& pool = *commandPools[i];
            VK::CommandBuffer& cmd = *commandBuffers[i];
            const VK::Framebuffer& framebuffer = *VK::GetSwapChain().GetFramebuffers()[i];

            pool.Reset();
            cmd.Begin();
                cmd.BeginRenderPass(pipeline.GetRenderPass(), framebuffer);
                    cmd.BindPipeline(pipeline);
                        for (size_t j = 0; j < meshes.size(); ++j)
                        {
                            const uint32_t dynamicOffset {static_cast<uint32_t>(j * DynamicAlignment<VK::EntityUBO>::Get())};
                            cmd.BindVertexBuffers({ meshes[j].vertexBuffer->UnderlyingPtr() }, {0});
                                cmd.BindIndexBuffer(meshes[j].indexBuffer->UnderlyingRef());
                                    cmd.BindDescriptorSets(pipeline, 1, &meshes[j].descriptorSet->GetVkDescriptorSet(), 1, &dynamicOffset);
                                        cmd.DrawIndexed(meshes[j].numIndices, 1, 0, 0, 0);
                        }
                cmd.EndRenderPass();
            cmd.End();
        }
            
        (*sceneUniformBuffer)().proj = glm::perspective(glm::radians(70.0f), window->GetAspectRatio(), 0.1f, 1000.0f); 
        (*sceneUniformBuffer)().proj[1][1] *= -1.0f;
        (*sceneUniformBuffer).Overwrite();

        while (window->IsRunning())
        {
            window->BeginFrame();

            const float deltaTime {window->GetDeltaTime()};
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

            frameManager.AcquireSwapChainImage();

            static float theta {0}; theta += deltaTime;

            for (size_t i = 0; i < meshes.size(); ++i)
            {
                spaceObjects[i].SetRotationY(theta);
            }

            (*entityUniformBuffer).Overwrite();

            const VK::Frame& frame {*frameManager.GetCurrentFrame()};
	        const VkFence& fence {frame.GetInFlightFence()};
            const VkSemaphore& wait {frame.GetSemaphore(0)};
            const VkSemaphore& signal {frame.GetSemaphore(1)};
            const VK::CommandBuffer& cmd {*commandBuffers[VK::GetSwapChain().GetCurrentImageIndex()]};

            vkResetFences(VK::GetDevice().GetVkDevice(), 1, &fence);
            cmd.SubmitToQueue(VK::Queues::graphicsQueue, &wait, &signal, fence);

            frameManager.Present();

            window->EndFrame();
        }

        VK::GetDevice().WaitIdle();
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}