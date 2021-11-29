#include "Renderer.h"

#include "../Pipeline/VertexLayouts/Layouts.h"
#include "../RenderTarget/RenderTarget.h"
#include "../Descriptors/DescriptorSet.h"
#include "../SwapChain/SwapChain.h"
#include <gtc/matrix_transform.hpp>
#include "../Device/QueueFamily.h"
#include "../Device/Device.h"

namespace VK
{
    void Renderer::CreateCmdEntities(const size_t numCmdBuffers)
    {
        commandBuffers.reserve(numCmdBuffers);
        commandPools.reserve(numCmdBuffers);
        for (size_t i = 0; i < numCmdBuffers; ++i)
        {
            Ptr<CommandPool> pool = CreatePtr<CommandPool>();
            commandBuffers.push_back(CreatePtr<CommandBuffer>(*pool));
            commandPools.push_back(std::move(pool));
        }
    }

    void Renderer::CreatePipeline(const Str& vertexShaderCode, const Str& fragmentShaderCode)
    {
        const Vec<VkDescriptorSetLayoutBinding> bindings({
            CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
            CreateBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC),
            CreateBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        });

        descriptorSetLayout = CreatePtr<DescriptorSetLayout>(bindings);
		const BindingDescriptions bindingDescriptors {Vertex::GetBindingDescriptions()};
		const AttributeDescriptions attributeDescriptors {Vertex::GetAttributeDescriptions()};

        pipeline = CreatePtr<Pipeline>(
            vertexShaderCode, 
            fragmentShaderCode,
            GetSwapChain().GetSize(),
            AttachmentDescriptions { GetSwapChain().GetDefaultAttachmentDescription(), Util::CreateDefaultDepthAttachment(device.FindDepthFormat()) },
            bindingDescriptors,
            attributeDescriptors,
            SetLayouts { descriptorSetLayout->GetVkDescriptorSetLayout() }
        );

        renderTarget = CreatePtr<RenderTarget>(GetSwapChain().GetSize(), GetSwapChain().GetImageViews(), pipeline->GetRenderPass());
    }

    void Renderer::CreateUniformBuffers()
    {        
        entityUniformBuffer = CreatePtr<EntityUniformBuffer<EntityUBO>>(50);
        sceneUniformBuffer = CreatePtr<SceneUniformBuffer<SceneUBO>>();
    }

    Renderer::Renderer(const Str& vertexShaderCode, const Str& fragmentShaderCode, const size_t numCmdBuffers, const Device& device) : device {device}
    {
        CreateCmdEntities(numCmdBuffers);
        CreatePipeline(vertexShaderCode, fragmentShaderCode);
        CreateUniformBuffers();
    }

    SpaceObject& Renderer::Add(const ::Util::ModelAsset& modelAsset, const ::Util::ImageAsset& imageAsset)
    {
        IRenderable* item = new IRenderable(
            modelAsset,
            imageAsset,
            *sceneUniformBuffer,
            *entityUniformBuffer,
            *descriptorSetLayout,
            renderable.size()
        );
        renderable.push_back(Ptr<IRenderable>(item));
        return item->GetSpaceObject();
    }

    void Renderer::Record(const Vec2ui size)
    {
        for (size_t i = 0; i < GetNumCmdBuffers(); ++i)
        {
            VK::CommandPool& pool = GetCommandPool(i);
            VK::CommandBuffer& cmd = GetCommandBuffer(i);
            const Framebuffer& framebuffer = renderTarget->GetFramebuffer(i);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = size.x;
            viewport.height = size.y;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = {static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y)};

            pool.Reset();
            cmd.Begin();

                vkCmdSetViewport(cmd.GetVkCommandBuffer(), 0, 1, &viewport);
                vkCmdSetScissor(cmd.GetVkCommandBuffer(), 0, 1, &scissor);

                cmd.BeginRenderPass(GetPipeline().GetRenderPass(), framebuffer);
                    cmd.BindPipeline(GetPipeline());
                        for (size_t j = 0; j < renderable.size(); ++j)
                        {
                            const uint32_t dynamicOffset {static_cast<uint32_t>(j * DynamicAlignment<VK::EntityUBO>::Get())};
                            cmd.BindVertexBuffers({renderable[j]->GetVertexBuffer().UnderlyingPtr()}, {0});
                                cmd.BindIndexBuffer(renderable[j]->GetIndexBuffer().UnderlyingRef());
                                    cmd.BindDescriptorSets(GetPipeline(), 1, &renderable[j]->GetDescriptorSet().GetVkDescriptorSet(), 1, &dynamicOffset);
                                        cmd.DrawIndexed(renderable[j]->GetNumIndices(), 1, 0, 0, 0);
                        }
                cmd.EndRenderPass();
            cmd.End();
        }        
    }

    void Renderer::UpdateUniformBuffers(const float ratio)
    {
        (*sceneUniformBuffer)().projection = glm::perspective(glm::radians(70.0f), ratio, 0.1f, 1000.0f); 
        (*sceneUniformBuffer)().projection[1][1] *= -1.0f;
        (*sceneUniformBuffer).Overwrite();

        (*entityUniformBuffer).Overwrite();
    }

    void Renderer::Render(const Frame& frame, const uint32_t swapChainImageIndex)
    {
        // Todo: Do smth with sync-objects.
        const VkFence& fence = frame.GetInFlightFence();
        const VkSemaphore& wait = frame.GetSemaphore(0);
        const VkSemaphore& signal = frame.GetSemaphore(1);

        const CommandBuffer& cmd = *commandBuffers[swapChainImageIndex];
        vkResetFences(device.GetVkDevice(), 1, &fence);
        cmd.SubmitToQueue(Queues::graphicsQueue, &wait, &signal, fence);
    }

    CommandBuffer& Renderer::GetCommandBuffer(const size_t i)
    {
        return *commandBuffers[i];
    }

    CommandPool& Renderer::GetCommandPool(const size_t i)
    {
        return *commandPools[i];
    }

    const Pipeline& Renderer::GetPipeline() const
    {
        return *pipeline;
    }

    const DescriptorSetLayout& Renderer::GetDescriptorSetLayout() const
    {
        return *descriptorSetLayout;
    }

    size_t Renderer::GetNumCmdBuffers() const
    {
        return commandBuffers.size();
    }

    size_t Renderer::GetNumCmdPool() const
    {
        return commandPools.size();
    }

    EntityUniformBuffer<EntityUBO>& Renderer::GetEntityUBO()
    {
        return *entityUniformBuffer;
    }

    SceneUniformBuffer<SceneUBO>& Renderer::GetSceneUBO()
    {
        return *sceneUniformBuffer;
    }

    size_t Renderer::GetNumRenderableEntities() const
    {
        return renderable.size();
    }

    SpaceObject& Renderer::GetSpaceObject(const size_t i)
    {
        return renderable[i]->GetSpaceObject();
    }
}