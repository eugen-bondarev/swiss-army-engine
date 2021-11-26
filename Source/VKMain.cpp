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

struct PerEntityUBO
{
    glm::mat4x4 model;
    float another;
};

struct Mesh
{
    Mesh(const Util::ModelAsset& modelAsset, const Util::ImageAsset& imageAsset, const VK::DescriptorSetLayout& descriptorSetLayout, const VK::Buffer& globalUBO, const VK::Buffer& localUBO, PerEntityUBO& ubo) : ubo{ubo}
    {
        const VK::Buffer stagingVertexBuffer(modelAsset.vertices);
        vertexBuffer = CreatePtr<VK::Buffer>(stagingVertexBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        const VK::Buffer stagingIndexBuffer(modelAsset.indices);
        indexBuffer = CreatePtr<VK::Buffer>(stagingIndexBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        texture = CreatePtr<VK::Texture2D>(imageAsset.size, 4, imageAsset.data);
        
        numIndices = modelAsset.indices.size();

        descriptorSet = CreatePtr<VK::DescriptorSet>(
            VK::GetDefaultDescriptorPool(),
            std::vector<VkDescriptorSetLayout> {descriptorSetLayout.GetVkDescriptorSetLayout()}
        );

		const std::vector<VkWriteDescriptorSet> writeDescriptorSets = 
		{
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &globalUBO.GetVkDescriptor()),
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &localUBO.GetVkDescriptor()),
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &texture->GetImageView().GetVkDescriptor())
		};

		descriptorSet->Update(writeDescriptorSets);

        ApplyTransform();
    }

    void ApplyTransform()
    {
        ubo.model = glm::translate(glm::mat4x4(1), position);
        ubo.model = glm::rotate(ubo.model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        ubo.model = glm::rotate(ubo.model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        ubo.model = glm::rotate(ubo.model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    }

    void SetPosition(const Vec3f position) { this->position = position; ApplyTransform(); }
    void SetRotation(const Vec3f rotation) { this->rotation = rotation; ApplyTransform(); }
    void SetAnother(const float another) { this->ubo.another = another; }

    Ptr<VK::DescriptorSet> descriptorSet;
    Ptr<VK::Buffer> vertexBuffer;
    Ptr<VK::Buffer> indexBuffer;
    Ptr<VK::Texture2D> texture;
    unsigned int numIndices;

    PerEntityUBO& ubo;

private:
    Vec3f position{0};
    Vec3f rotation{0};
};

constexpr unsigned int numInstances{4};

template<typename... Args>
std::string string_format(const std::string& format, Args... args)
{
    const int size_s{std::snprintf(nullptr, 0, format.c_str(), args...) + 1};

    if (size_s <= 0) 
    { 
        throw std::runtime_error( "Error during formatting." ); 
    }

    const size_t size{static_cast<size_t>(size_s)};
    const std::unique_ptr<char[]> buf{std::make_unique<char[]>(size)};
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}

void CompileGLSL(const std::string& pathToGLSL, const std::string& pathToOutput)
{
    
}

int main()
{
    try
    {
        Util::Path path = "hello, world!";
        LINE_OUT(path());

        const std::string compileVertexShaderCommand{string_format("glslc %s/Assets/Shaders/VertexShader.vert -o %s/Assets/Shaders/VertexShader.vert.com", PROJECT_ROOT_DIR, PROJECT_ROOT_DIR)};
        const int result{system(compileVertexShaderCommand.c_str())};

        const Util::TextAsset vertexShaderCode = Util::LoadTextFile("Assets/Shaders/VertexShader.vert.com", true);
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile("Assets/Shaders/FragmentShader.frag.spv", true);

        const Util::ModelAsset characterMesh = Util::LoadModelFile("Assets/Models/CharacterModel.fbx");
        const Util::ImageAsset characterTexture = Util::LoadImageFile("Assets/Images/CharacterTexture.png");

        Ptr<API::Window> window = CreatePtr<API::Window>(API::Type::Vulkan, WindowMode::Windowed, true, Vec2ui {1024, 768});

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

        const std::vector<VkDescriptorSetLayoutBinding> bindings = 
        {
            VK::CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
            VK::CreateBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC),
            VK::CreateBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        const VK::DescriptorSetLayout descriptorSetLayout(bindings);

		const VK::AttachmentDescriptions attachments = { VK::Util::CreateAttachment(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) };
		const VK::BindingDescriptions bindingDescriptors = VK::Vertex::GetBindingDescriptions();
		const VK::AttributeDescriptions attributeDescriptors = VK::Vertex::GetAttributeDescriptions();
		const std::vector<VkDescriptorSetLayout> descriptorSetLayouts = { descriptorSetLayout.GetVkDescriptorSetLayout() };

        VK::Pipeline pipeline(
            vertexShaderCode, 
            pixelShaderCode,
            window->GetSize(),
            { 
                VK::Util::CreateAttachment(
                    VK::GetSwapChain().GetImageFormat()
                ),
                VK::Util::CreateAttachment(
                    depthImage.GetVkFormat(),
                    VK_IMAGE_LAYOUT_UNDEFINED, 
                    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 
                    VK_ATTACHMENT_LOAD_OP_CLEAR, 
                    VK_ATTACHMENT_STORE_OP_DONT_CARE,
                    VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                    VK_ATTACHMENT_STORE_OP_DONT_CARE,
                    VK_SAMPLE_COUNT_1_BIT
                )
            },
            bindingDescriptors,
            attributeDescriptors,
            descriptorSetLayouts
        );

	    VK::GetSwapChain().InitFramebuffers(pipeline.GetRenderPass(), depthImageView);

        VK::SceneUniformBuffer<PerSceneUBO> sceneUniformBuffer;
        VK::EntityUniformBuffer<PerEntityUBO> entityUniformBuffer(numInstances);

        std::vector<Mesh> meshes;
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout, sceneUniformBuffer, entityUniformBuffer, entityUniformBuffer()[0]);
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout, sceneUniformBuffer, entityUniformBuffer, entityUniformBuffer()[1]);
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout, sceneUniformBuffer, entityUniformBuffer, entityUniformBuffer()[2]);
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout, sceneUniformBuffer, entityUniformBuffer, entityUniformBuffer()[3]);

        meshes[0].SetPosition({-5, -5, -15});
        meshes[1].SetPosition({-5, -5, -25});
        meshes[2].SetPosition({ 5, -5, -15});
        meshes[3].SetPosition({ 5, -5, -25});

        meshes[0].SetAnother(0);
        meshes[1].SetAnother(2);
        meshes[2].SetAnother(4);
        meshes[3].SetAnother(6);

        meshes[0].SetRotation({0,  45, 0});
        meshes[1].SetRotation({0, -45, 0});

        while (window->IsRunning())
        {
            window->BeginFrame();

            frameManager.AcquireSwapChainImage();

            const VK::Frame* frame = frameManager.GetCurrentFrame();
            const VkSemaphore* wait = &frame->GetSemaphore(0);
            const VkSemaphore* signal = &frame->GetSemaphore(1);

            VK::CommandPool& pool = *commandPools[VK::GetSwapChain().GetCurrentImageIndex()];
            VK::CommandBuffer& cmd = *commandBuffers[VK::GetSwapChain().GetCurrentImageIndex()];
            const VK::Framebuffer& framebuffer = VK::GetSwapChain().GetCurrentScreenFramebuffer();

            static float theta{0}; theta += 0.025f;
            
            sceneUniformBuffer().proj = glm::perspective(glm::radians(70.0f), window->GetAspectRatio(), 0.1f, 1000.0f); 
            sceneUniformBuffer().proj[1][1] *= -1.0f;

            for (size_t i = 0; i < meshes.size(); ++i)
            {
                meshes[i].SetRotation({0, theta, 0});
            }

            sceneUniformBuffer.Overwrite();
            entityUniformBuffer.Overwrite();
            
            pool.Reset();
                cmd.Begin();
                    cmd.BeginRenderPass(pipeline.GetRenderPass(), framebuffer);
                        cmd.BindPipeline(pipeline);

                            for (size_t i = 0; i < meshes.size(); ++i)
                            {
                                const uint32_t dynamicOffset = i * DynamicAlignment<PerEntityUBO>::Get();
                                cmd.BindVertexBuffers({meshes[i].vertexBuffer.get()}, {0});
                                cmd.BindIndexBuffer(*meshes[i].indexBuffer);
                                    cmd.BindDescriptorSets(pipeline, 1, &meshes[i].descriptorSet->GetVkDescriptorSet(), 1, &dynamicOffset);
                                        vkCmdDrawIndexed(cmd.GetVkCommandBuffer(), meshes[i].numIndices, 1, 0, 0, 0);
                            }
                    cmd.EndRenderPass();
                cmd.End();

	        const VkFence fence = frame->GetInFlightFence();
            vkResetFences(VK::GetDevice().GetVkDevice(), 1, &fence);
            cmd.SubmitToQueue(VK::Queues::graphicsQueue, wait, signal, fence);

            frameManager.Present();
        }

        VK::GetDevice().WaitIdle();
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}