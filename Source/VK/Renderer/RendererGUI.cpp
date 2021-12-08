#include "RendererGUI.h"

#include "../SwapChain/SwapChain.h"
#include "../Device/QueueFamily.h"
#include <imgui_impl_vulkan.h>
#include "../Device/Device.h"

namespace VK
{
    RendererGUI::RendererGUI(
        const std::string& vertexShaderCode, 
        const std::string& fragmentShaderCode, 
        const size_t numCmdBuffers, 
        const size_t samples,
        const RendererFlags flags,
        GraphicsContext& ctx
    ) : Renderer(numCmdBuffers, samples, flags, ctx)
    {
        needsResize.resize(GetNumCmdBuffers());

        for (size_t i = 0; i < needsResize.size(); ++i) needsResize[i] = true;
        newSize = ctx.GetWindow().GetSize();

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newViewportSize)
        {
            for (size_t i = 0; i < needsResize.size(); ++i) needsResize[i] = true;
            newSize = newViewportSize;
        });

        CreateUniformBuffers();

        CreateGraphicsResources(
            vertexShaderCode,
            fragmentShaderCode,
            samples,
            flags
        );
    }

    SpaceObject& RendererGUI::Add(const ::Util::ModelAsset<PredefinedVertexLayouts::Vertex2D>& modelAsset, const ::Util::ImageAsset& imageAsset)
    {
        IRenderable<PredefinedVertexLayouts::Vertex2D>* item = new IRenderable<PredefinedVertexLayouts::Vertex2D>(
            modelAsset,
            imageAsset,
            *sceneUniformBuffer,
            *entityUniformBuffer,
            *descriptorSetLayout,
            renderable.size()
        );
        renderable.push_back(Ptr<IRenderable<PredefinedVertexLayouts::Vertex2D>>(item));
        return item->GetSpaceObject();
    }

    void RendererGUI::CreateUniformBuffers()
    {        
        entityUniformBuffer = CreatePtr<EntityUniformBuffer<EntityUBO>>(50);
        sceneUniformBuffer = CreatePtr<SceneUniformBuffer<SceneUBO>>();
        orthogonalSpace = CreatePtr<OrthogonalSpace>(&(*sceneUniformBuffer)());
    }

    void RendererGUI::UpdateUniformBuffers(const float ratio)
    {
        (*sceneUniformBuffer).Overwrite();
        (*entityUniformBuffer).Overwrite();
    }

    void RendererGUI::CreateGraphicsResources(
        const std::string& vertexShaderCode, 
        const std::string& fragmentShaderCode, 
        const size_t samples,
        const RendererFlags flags
    )
    {
        const std::vector<VkDescriptorSetLayoutBinding> bindings({
            CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
            CreateBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC),
            CreateBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        });

        descriptorSetLayout = CreatePtr<DescriptorSetLayout>(bindings);
		const BindingDescriptions bindingDescriptors {Vertex2D::GetBindingDescriptions()};
		const AttributeDescriptions attributeDescriptors {Vertex2D::GetAttributeDescriptions()};

        AttachmentDescriptions attachments;
        VkAttachmentDescription swapChainAttachment = GetSwapChain().GetDefaultAttachmentDescription(SamplesToVKFlags(samples));

        swapChainAttachment.finalLayout = 
            flags & RendererFlags_Output ?
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR :
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        swapChainAttachment.initialLayout =
            flags & RendererFlags_Load ?
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL :
                VK_IMAGE_LAYOUT_UNDEFINED;

        swapChainAttachment.loadOp =
            flags & RendererFlags_Load ?
                VK_ATTACHMENT_LOAD_OP_LOAD :
                VK_ATTACHMENT_LOAD_OP_DONT_CARE;

        attachments.push_back(swapChainAttachment);

        if (flags & RendererFlags_UseDepth)
        {
            const VkAttachmentDescription depthAttachment = Util::CreateDefaultDepthAttachment(ctx.GetDevice().FindDepthFormat(), SamplesToVKFlags(samples));
            attachments.push_back(depthAttachment);
        }

        if (samples > 1)
        {
            VkAttachmentDescription colorAttachmentResolve{};
            colorAttachmentResolve.format = GetSwapChain().GetImageFormat();
            colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

            colorAttachmentResolve.loadOp = swapChainAttachment.loadOp;
            colorAttachmentResolve.initialLayout = swapChainAttachment.initialLayout;
            
            colorAttachmentResolve.finalLayout = 
                flags & RendererFlags_Output ? 
                    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : 
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            attachments.push_back(colorAttachmentResolve);
        }

        pipeline = CreatePtr<Pipeline>(
            vertexShaderCode, 
            fragmentShaderCode,
            GetSwapChain().GetSize(),
            attachments,
            bindingDescriptors,
            attributeDescriptors,
            SetLayouts { descriptorSetLayout->GetVkDescriptorSetLayout() },
            samples,
            flags & RendererFlags_UseDepth,
            ctx.GetDevice()
        );

        renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), pipeline->GetRenderPass(), samples, flags & RendererFlags_UseDepth);

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newSize)
        {
            // space->SetAspectRatio(newSize.x / newSize.y);
            vkQueueWaitIdle(Queues::graphicsQueue);
            renderTarget.reset();
            renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), pipeline->GetRenderPass(), samples, flags & RendererFlags_UseDepth);
        });
    }

    void RendererGUI::Record(const size_t cmdIndex)
    {
        VK::CommandPool& pool = GetCommandPool(cmdIndex);
        VK::CommandBuffer& cmd = GetCommandBuffer(cmdIndex);
        const Framebuffer& framebuffer = renderTarget->GetFramebuffer(cmdIndex);

        pool.Reset();
        cmd.Begin();

            if (needsResize[cmdIndex])
            {
                cmd.SetViewport(newSize.x, newSize.y);
                cmd.SetScissors(newSize.x, newSize.y);
                needsResize[cmdIndex] = false;
            }

            cmd.BeginRenderPass(pipeline->GetRenderPass(), framebuffer);
                cmd.BindPipeline(*pipeline);
                    for (size_t i = 0; i < renderable.size(); ++i)
                    {
                        const uint32_t dynamicOffset {static_cast<uint32_t>(i * DynamicAlignment<VK::EntityUBO>::Get())};
                        cmd.BindVertexBuffers({renderable[i]->GetVertexBuffer().UnderlyingPtr()}, {0});
                            cmd.BindIndexBuffer(renderable[i]->GetIndexBuffer().UnderlyingRef());
                                cmd.BindDescriptorSets(*pipeline, 1, &renderable[i]->GetDescriptorSet().GetVkDescriptorSet(), 1, &dynamicOffset);
                                    cmd.DrawIndexed(renderable[i]->GetNumIndices(), 1, 0, 0, 0);
                    }
            cmd.EndRenderPass();
        cmd.End();
    }

    EntityUniformBuffer<EntityUBO>& RendererGUI::GetEntityUBO()
    {
        return *entityUniformBuffer;
    }

    SceneUniformBuffer<SceneUBO>& RendererGUI::GetSceneUBO()
    {
        return *sceneUniformBuffer;
    }

    size_t RendererGUI::GetNumRenderableEntities() const
    {
        return renderable.size();
    }

    SpaceObject& RendererGUI::GetSpaceObject(const size_t i)
    {
        return renderable[i]->GetSpaceObject();
    }

    OrthogonalSpace& RendererGUI::GetOrthogonalSpace()
    {
        return *orthogonalSpace;
    }
}