#include "Renderer.h"

#include "../Pipeline/VertexLayouts/Layouts.h"
#include "../RenderTarget/RenderTarget.h"
#include "../Descriptors/DescriptorSet.h"
#include "../SwapChain/SwapChain.h"
#include <gtc/matrix_transform.hpp>
#include "../Device/QueueFamily.h"
#include "../GraphicsContext.h"
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

    // void Renderer::CreateGraphicsResources(const Str& vertexShaderCode, const Str& fragmentShaderCode, const size_t samples, const bool useDepth, const bool isOutput)
    // {
    //     const Vec<VkDescriptorSetLayoutBinding> bindings({
    //         CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
    //         CreateBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC),
    //         CreateBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
    //     });

    //     descriptorSetLayout = CreatePtr<DescriptorSetLayout>(bindings);
	// 	const BindingDescriptions bindingDescriptors {Vertex::GetBindingDescriptions()};
	// 	const AttributeDescriptions attributeDescriptors {Vertex::GetAttributeDescriptions()};

    //     AttachmentDescriptions attachments;
    //     VkAttachmentDescription swapChainAttachment = GetSwapChain().GetDefaultAttachmentDescription(SamplesToVKFlags(samples));
    //     if (isOutput)
    //     {
    //         swapChainAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //         swapChainAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    //         swapChainAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    //     }
    //     else
    //     {
    //         swapChainAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //         swapChainAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //     }
    //     attachments.push_back(swapChainAttachment);

    //     if (useDepth)
    //     {
    //         const VkAttachmentDescription depthAttachment = Util::CreateDefaultDepthAttachment(ctx.GetDevice().FindDepthFormat(), SamplesToVKFlags(samples));
    //         attachments.push_back(depthAttachment);
    //     }

    //     if (samples > 1)
    //     {
    //         VkAttachmentDescription colorAttachmentResolve{};
    //         colorAttachmentResolve.format = GetSwapChain().GetImageFormat();
    //         colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    //         colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //         colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    //         colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //         colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //         colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //         colorAttachmentResolve.finalLayout = isOutput ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //         attachments.push_back(colorAttachmentResolve);
    //     }

    //     pipeline = CreatePtr<Pipeline>(
    //         vertexShaderCode, 
    //         fragmentShaderCode,
    //         GetSwapChain().GetSize(),
    //         attachments,
    //         bindingDescriptors,
    //         attributeDescriptors,
    //         SetLayouts { descriptorSetLayout->GetVkDescriptorSetLayout() },
    //         samples,
    //         useDepth,
    //         ctx.GetDevice()
    //     );

    //     renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), pipeline->GetRenderPass(), samples, useDepth);

    //     ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newSize)
    //     {
    //         space->SetAspectRatio(newSize.x / newSize.y);
    //         vkQueueWaitIdle(Queues::graphicsQueue);
    //         renderTarget.reset();
    //         renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), pipeline->GetRenderPass(), samples, useDepth);
    //     });
    // }

    void Renderer::CreateUniformBuffers()
    {        
        entityUniformBuffer = CreatePtr<EntityUniformBuffer<EntityUBO>>(50);
        sceneUniformBuffer = CreatePtr<SceneUniformBuffer<SceneUBO>>();
        space = CreatePtr<Space>(&sceneUniformBuffer->operator()());
    }

    Renderer::Renderer(const Str& vertexShaderCode, const Str& fragmentShaderCode, const size_t numCmdBuffers, const size_t samples, const bool useDepth, const bool isOutput, GraphicsContext& ctx) : ctx {ctx}
    {
        CreateCmdEntities(numCmdBuffers);
        CreateUniformBuffers();
        space->SetAspectRatio(ctx.GetWindow().GetAspectRatio());

        // CreateGraphicsResources(vertexShaderCode, fragmentShaderCode, samples, useDepth, isOutput);
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

    void Renderer::RecordAll()
    {
        for (size_t i = 0; i < GetNumCmdBuffers(); ++i)
        {
            Record(i);
        }
    }

    void Renderer::UpdateUniformBuffers(const float ratio)
    {
        (*sceneUniformBuffer).Overwrite();
        (*entityUniformBuffer).Overwrite();
    }

    void Renderer::Render(const Frame& frame, const uint32_t swapChainImageIndex, const bool resetFence, const uint32_t waitSemaphoreIndex, const uint32_t signalSemaphoreIndex)
    {
        // Todo: Do smth with sync-objects.
        const VkFence& fence = frame.GetInFlightFence();
        const VkSemaphore& wait = frame.GetSemaphore(waitSemaphoreIndex);
        const VkSemaphore& signal = frame.GetSemaphore(signalSemaphoreIndex);

        const CommandBuffer& cmd = *commandBuffers[swapChainImageIndex];
        if (resetFence)
        {
            vkResetFences(ctx.GetDevice().GetVkDevice(), 1, &fence);
            cmd.SubmitToQueue(Queues::graphicsQueue, &wait, &signal, fence);
        }
        else
        {
            cmd.SubmitToQueue(Queues::graphicsQueue, &wait, &signal);
        }
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

    Space& Renderer::GetSpace()
    {
        return *space;
    }
}