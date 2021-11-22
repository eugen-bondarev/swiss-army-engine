#include "Util/Assets.h"
#include "API/Window.h"
#include "VK/vk.h"
#include "VK/frame/frame.h"
#include "VK/pipeline/pipeline.h"
#include "VK/device/queue_family.h"
#include "VK/pipeline/render_pass.h"
#include "VK/commands/command_buffer.h"
#include "VK/descriptors/descriptor_set.h"
#include "VK/descriptors/descriptor_set_layout.h"
#include "VK/image/texture2d.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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

        Ptr<API::Window> window = CreatePtr<API::Window>(API::Type::Vulkan, WindowMode::Windowed, true, 800, 600);

        VK::Bootstrap(window->GetHandle());

        VK::FrameManager frameManager(0, 1, 2, 2);
        VK::RenderPass renderPass(
            {
                VK::Util::CreateAttachment(
                    VK::Global::swapChain->GetImageFormat(), 
                    VK_IMAGE_LAYOUT_UNDEFINED, 
                    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 
                    VK_ATTACHMENT_LOAD_OP_CLEAR, 
                    VK_ATTACHMENT_STORE_OP_STORE
                )
            }
        );

        std::vector<VK::CommandPool*> commandPools;
        std::vector<VK::CommandBuffer*> commandBuffers;

        for (size_t i = 0; i < VK::Global::swapChain->GetImageViews().size(); ++i)
        {
            VK::CommandPool* pool = new VK::CommandPool();
            commandBuffers.push_back(new VK::CommandBuffer(pool));
            commandPools.push_back(pool);
        }

	    VK::Global::swapChain->InitFramebuffers(renderPass.GetVkRenderPass());

        std::vector<VkDescriptorSetLayoutBinding> bindings = 
        {
            VK::CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
            VK::CreateBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        VK::DescriptorSetLayout descriptorSetLayout(bindings);

		VK::AttachmentDescriptions attachments = { VK::Util::CreateAttachment(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) };
		VK::BindingDescriptions binding_descriptors = VK::Vertex::GetBindingDescriptions();
		VK::AttributeDescriptions attribute_descriptors = VK::Vertex::GetAttributeDescriptions();
		std::vector<VkDescriptorSetLayout> descriptor_set_layouts = { descriptorSetLayout.GetVkDescriptorSetLayout() };

        VK::Pipeline pipeline(
            vertexShaderCode, pixelShaderCode,
            VK::Vec2{800, 600},
            std::vector<VkAttachmentDescription> { VK::Util::CreateAttachment(
                VK::Global::swapChain->GetImageFormat()
            ) },
            binding_descriptors,
            attribute_descriptors,
            descriptor_set_layouts
        );

		VK::Buffer stagingVertexBuffer(characterMesh.vertices);
		VK::Buffer vertexBuffer(&stagingVertexBuffer);

		VK::Buffer stagingIndexBuffer(characterMesh.indices);
		VK::Buffer indexBuffer(&stagingIndexBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        glm::mat4 pre = glm::mat4(1);
        pre[1][1] = -1.0f;
        uboData.proj = pre * glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 1000.0f) * glm::translate(glm::mat4x4(1), glm::vec3(0, -5, -10));

        VK::Buffer ubo(
			sizeof(UBO),
			1,
			&uboData,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
        );

        VK::Texture2D texture(
            VK::Vec2{(float)characterTexture.width, (float)characterTexture.height},
            4, characterTexture.data
        );

        VK::DescriptorSet descriptorSet(
            VK::Global::descriptorPool, 
            std::vector<VkDescriptorSetLayout> { descriptorSetLayout.GetVkDescriptorSetLayout() }
        );

		std::vector<VkWriteDescriptorSet> offscreen_write_descriptor_sets = 
		{
			VK::CreateWriteDescriptorSet(&descriptorSet, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &ubo.GetDescriptor()),
			VK::CreateWriteDescriptorSet(&descriptorSet, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &texture.GetImageView()->GetDescriptor())
		};

		descriptorSet.Update(offscreen_write_descriptor_sets);

        while (window->IsRunning())
        {
            window->BeginFrame();

            frameManager.AcquireSwapChainImage();

            VK::Frame* frame = frameManager.GetCurrentFrame();
            VkSemaphore* wait = &frame->GetSemaphore(0);
            VkSemaphore* signal = &frame->GetSemaphore(1);

            VK::CommandPool* pool = commandPools[VK::Global::swapChain->GetCurrentImageIndex()];
            VK::CommandBuffer* cmd = commandBuffers[VK::Global::swapChain->GetCurrentImageIndex()];
            VK::Framebuffer* framebuffer = VK::Global::swapChain->GetCurrentScreenFramebuffer();

            static float theta{0}; theta += 0.01f;
            
            glm::mat4 pre = glm::mat4(1);
            pre[1][1] = -1.0f;
            uboData.proj = pre * 
                glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 1000.0f) * 
                glm::translate(glm::mat4x4(1), glm::vec3(0, -5, -10)) *
                glm::rotate(glm::mat4x4(1), glm::radians(theta), glm::vec3(0, 1, 0));
            ubo.Update(&uboData);
            
            pool->Reset();
                cmd->Begin();
                    cmd->BeginRenderPass(&renderPass, framebuffer);
                        cmd->BindPipeline(&pipeline);
                            cmd->BindVertexBuffers({ &vertexBuffer }, { 0 });
                            cmd->BindIndexBuffer(&indexBuffer);
				                cmd->BindDescriptorSets(&pipeline, 1, &descriptorSet.GetVkDescriptorSet());
                                vkCmdDrawIndexed(cmd->GetVkCommandBuffer(), characterMesh.indices.size(), 1, 0, 0, 0);
				                // cmd->DrawIndexed(canvas.indexBuffer->GetAmountOfElements(), 1, 0, 0, 0);
                    cmd->EndRenderPass();
                cmd->End();


	        VkFence fence = frame->GetInFlightFence();
            vkResetFences(VK::Global::device->GetVkDevice(), 1, &fence);
            cmd->SubmitToQueue(VK::Global::Queues::graphicsQueue, wait, signal, fence);

            frameManager.Present();
            // window->EndFrame();
        }

        VK::Global::device->WaitIdle();

        for (size_t i = 0; i < commandPools.size(); ++i)
        {
            delete commandBuffers[i];
            delete commandPools[i];
        }
    }
    catch (const std::runtime_error& exception)
    {
        MessageBox(nullptr, exception.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    VK::Shutdown();

    return 0;
}