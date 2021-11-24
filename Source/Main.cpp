#include "Util/Assets.h"
#include "API/Window.h"
#include "VK/VK.h"

#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

struct UBO
{
    glm::mat4x4 proj;
} uboData;

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
            commandBuffers.push_back(CreatePtr<VK::CommandBuffer>(pool.get()));
            commandPools.push_back(std::move(pool));
        }

        const std::vector<VkDescriptorSetLayoutBinding> bindings = 
        {
            VK::CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
            VK::CreateBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        VK::DescriptorSetLayout descriptorSetLayout(bindings);

		VK::AttachmentDescriptions attachments = { VK::Util::CreateAttachment(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) };
		VK::BindingDescriptions bindingDescriptors = VK::Vertex::GetBindingDescriptions();
		VK::AttributeDescriptions attributeDescriptors = VK::Vertex::GetAttributeDescriptions();
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts = { descriptorSetLayout.GetVkDescriptorSetLayout() };

        VK::Pipeline pipeline(
            vertexShaderCode, pixelShaderCode,
            window->GetSize(),
            std::vector<VkAttachmentDescription> { VK::Util::CreateAttachment(
                VK::GetSwapChain().GetImageFormat()
            ),            
            VK::Util::CreateAttachment(
                VK::GetDevice().FindDepthFormat(), 
                VK_IMAGE_LAYOUT_UNDEFINED, 
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 
                VK_ATTACHMENT_LOAD_OP_CLEAR, 
                VK_ATTACHMENT_STORE_OP_DONT_CARE,
                VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                VK_ATTACHMENT_STORE_OP_DONT_CARE,
                VK_SAMPLE_COUNT_1_BIT
            )},
            bindingDescriptors,
            attributeDescriptors,
            descriptorSetLayouts
        );

	    VK::GetSwapChain().InitFramebuffers(pipeline.GetRenderPass(), depthImageView);

		VK::Buffer stagingVertexBuffer(characterMesh.vertices);
		VK::Buffer vertexBuffer(stagingVertexBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

		VK::Buffer stagingIndexBuffer(characterMesh.indices);
		VK::Buffer indexBuffer(stagingIndexBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        VK::Buffer ubo(sizeof(UBO), 1, &uboData, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

        VK::Texture2D texture(characterTexture.size, 4, characterTexture.data);

        VK::DescriptorSet descriptorSet(
            VK::GetDefaultDescriptorPool(), 
            std::vector<VkDescriptorSetLayout> { descriptorSetLayout.GetVkDescriptorSetLayout() }
        );

		std::vector<VkWriteDescriptorSet> writeDescriptorSets = 
		{
			VK::CreateWriteDescriptorSet(&descriptorSet, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &ubo.GetVkDescriptor()),
			VK::CreateWriteDescriptorSet(&descriptorSet, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &texture.GetImageView()->GetVkDescriptor())
		};

		descriptorSet.Update(writeDescriptorSets);

        while (window->IsRunning())
        {
            window->BeginFrame();

            frameManager.AcquireSwapChainImage();

            const VK::Frame* frame = frameManager.GetCurrentFrame();
            const VkSemaphore* wait = &frame->GetSemaphore(0);
            const VkSemaphore* signal = &frame->GetSemaphore(1);

            VK::CommandPool* pool = commandPools[VK::GetSwapChain().GetCurrentImageIndex()].get();
            VK::CommandBuffer* cmd = commandBuffers[VK::GetSwapChain().GetCurrentImageIndex()].get();
            VK::Framebuffer* framebuffer = VK::GetSwapChain().GetCurrentScreenFramebuffer();

            static float theta{0}; theta += 0.025f;
            
            glm::mat4 pre = glm::mat4(1);
            pre[1][1] = -1.0f;
            uboData.proj = pre * 
                glm::perspective(glm::radians(70.0f), window->GetAspectRatio(), 0.1f, 1000.0f) * 
                glm::translate(glm::mat4x4(1), glm::vec3(0, -5, -10)) *
                glm::rotate(glm::mat4x4(1), glm::radians(theta), glm::vec3(0, 1, 0));
            ubo.Update(&uboData);
            
            pool->Reset();
                cmd->Begin();
                    cmd->BeginRenderPass(pipeline.GetRenderPass(), framebuffer);
                        cmd->BindPipeline(&pipeline);
                            cmd->BindVertexBuffers({ &vertexBuffer }, { 0 });
                            cmd->BindIndexBuffer(&indexBuffer);
				                cmd->BindDescriptorSets(&pipeline, 1, &descriptorSet.GetVkDescriptorSet());
                                vkCmdDrawIndexed(cmd->GetVkCommandBuffer(), characterMesh.indices.size(), 1, 0, 0, 0);
                    cmd->EndRenderPass();
                cmd->End();

	        VkFence fence = frame->GetInFlightFence();
            vkResetFences(VK::GetDevice().GetVkDevice(), 1, &fence);
            cmd->SubmitToQueue(VK::Queues::graphicsQueue, wait, signal, fence);

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