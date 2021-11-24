#include "Util/Aligned.h"
#include "Util/Assets.h"
#include "API/Window.h"
#include "VK/VK.h"

#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

struct PerSceneUBO
{
    glm::mat4x4 proj;
} perSceneUBO;

struct PerObjectUBO
{
    PerObjectUBO(const size_t numInstances) : model{numInstances}
    {
    }

    Aligned<glm::mat4x4> model;
};

struct Mesh
{
    Mesh(const Util::ModelAsset& modelAsset, const Util::ImageAsset& imageAsset, const VK::DescriptorSetLayout& descriptorSetLayout, const VK::Buffer& globalUBO, const VK::Buffer& localUBO)
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

		std::vector<VkWriteDescriptorSet> writeDescriptorSets = 
		{
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &globalUBO.GetVkDescriptor()),
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &localUBO.GetVkDescriptor()),
			VK::CreateWriteDescriptorSet(descriptorSet.get(), 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &texture->GetImageView().GetVkDescriptor())
		};

		descriptorSet->Update(writeDescriptorSets);
    }

    Ptr<VK::DescriptorSet> descriptorSet;
    Ptr<VK::Buffer> vertexBuffer;
    Ptr<VK::Buffer> indexBuffer;
    Ptr<VK::Texture2D> texture;
    unsigned int numIndices;

    Vec3f position{0, -5, -10};
    Vec3f rotation{0, 1, 0};
};

constexpr unsigned int numInstances{2};

int main()
{
    try
    {
        const Util::TextAsset vertexShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/VertexShader.vert.spv", true);
        const Util::TextAsset pixelShaderCode = Util::LoadTextFile(PROJECT_ROOT_DIR "/Assets/Shaders/FragmentShader.frag.spv", true);

        const Util::ModelAsset characterMesh = Util::LoadModelFile(PROJECT_ROOT_DIR "/Assets/Models/CharacterModel.fbx");
        const Util::ImageAsset characterTexture = Util::LoadImageFile(PROJECT_ROOT_DIR "/Assets/Images/CharacterTexture.png");

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

        VK::Buffer ubo(sizeof(PerSceneUBO), 1, &perSceneUBO, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

        // perObjectUBO.model = new Aligned<glm::mat4x4>(1);
        Ptr<PerObjectUBO> perObjectUBO = CreatePtr<PerObjectUBO>(numInstances);

        VK::Buffer localBuffer(numInstances * Aligned<glm::mat4x4>::dynamicAlignment, 1, perObjectUBO->model.data, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
		localBuffer.SetDescriptor(Aligned<glm::mat4x4>::dynamicAlignment);

		// auto& dyn = uboInstanceBuffer->GetDescriptor();

        // localBuffer.SetDescriptor(Aligned<glm::mat4x4>::dynamicAlignment);

        std::vector<Mesh> meshes;
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout, ubo, localBuffer);
        meshes.emplace_back(characterMesh, characterTexture, descriptorSetLayout, ubo, localBuffer);

        meshes[0].position.x -= 3;
        meshes[1].position.x += 3;

        meshes[0].rotation = {0, 1, 0};
        meshes[1].rotation = {0, -1, 0};

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
            
            glm::mat4 pre = glm::mat4(1);
            pre[1][1] = -1.0f;
            perSceneUBO.proj = pre * glm::perspective(glm::radians(70.0f), window->GetAspectRatio(), 0.1f, 1000.0f); 

            ubo.Update(&perSceneUBO);

            for (size_t i = 0; i < meshes.size(); ++i)
            {
                perObjectUBO->model[i] = glm::translate(glm::mat4x4(1), meshes[i].position) * glm::rotate(glm::mat4x4(1), glm::radians(theta), meshes[i].rotation);
            }
            localBuffer.Update(perObjectUBO->model.data);
            
            pool.Reset();
                cmd.Begin();
                    cmd.BeginRenderPass(pipeline.GetRenderPass(), framebuffer);
                        cmd.BindPipeline(pipeline);

                            for (size_t i = 0; i < meshes.size(); ++i)
                            {
                                uint32_t dynamicOffset = i * Aligned<glm::mat4x4>::dynamicAlignment;

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

    // delete perObjectUBO.model;

    return 0;
}