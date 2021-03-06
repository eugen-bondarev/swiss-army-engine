#include "Renderer3D.h"

#include "../SwapChain/SwapChain.h"
#include "../Device/QueueFamily.h"
#include <imgui_impl_vulkan.h>
#include "../Device/Device.h"

namespace VK
{
    Renderer3D::Renderer3D(
        const std::string& vsCode, 
        const std::string& fsCode, 
        const size_t samples,
        const RendererFlags flags,
        GraphicsContext& ctx
    ) : Renderer(GetSwapChain().GetNumBuffers(), samples, flags, ctx)
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
        CreateGraphicsResources(vsCode, fsCode);
    }

    SpaceObject& Renderer3D::Add(const ::Util::ModelAsset<PredefinedVertexLayouts::Vertex3D>& modelAsset, const ::Util::ImageAsset& imageAsset)
    {
        IRenderable<PredefinedVertexLayouts::Vertex3D>* item = new IRenderable<PredefinedVertexLayouts::Vertex3D>(
            modelAsset,
            imageAsset,
            *sceneUniformBuffer,
            *entityUniformBuffer,
            *descriptorSetLayout,
            renderable.size()
        );
        renderable.push_back(Ptr<IRenderable<PredefinedVertexLayouts::Vertex3D>>(item));
        return item->GetSpaceObject();
    }

    void Renderer3D::CreateUniformBuffers()
    {        
        entityUniformBuffer = CreatePtr<EntityUniformBuffer<EntityUBO>>(50);
        sceneUniformBuffer = CreatePtr<SceneUniformBuffer<SceneUBO>>();
        perspectiveSpace = CreatePtr<PerspectiveSpace>(sceneUniformBufferRequiresUpdate, &(*sceneUniformBuffer)());
    }

    void Renderer3D::UpdateUniformBuffers()
    {
        if (sceneUniformBufferRequiresUpdate)
        {
            (*sceneUniformBuffer).Overwrite();
            sceneUniformBufferRequiresUpdate = false;
        }

        // if (entityUniformBufferRequiresUpdate)
        {
            (*entityUniformBuffer).Overwrite();
            // entityUniformBufferRequiresUpdate = false;
        }
    }

    void Renderer3D::CreateGraphicsResources(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
    {
        const std::vector<VkDescriptorSetLayoutBinding> bindings({
            CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
            CreateBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC),
            CreateBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        });

        descriptorSetLayout = CreatePtr<DescriptorSetLayout>(bindings);
		const BindingDescriptions bindingDescriptors {Vertex::GetBindingDescriptions()};
		const AttributeDescriptions attributeDescriptors {Vertex::GetAttributeDescriptions()};

        AttachmentDescriptions attachments;

        VkAttachmentDescription colorAttachment = GetSwapChain().GetDefaultAttachmentDescription(SamplesToVKFlags(samples));
        colorAttachment.finalLayout = FlagsToFinalImageLayout(flags);
        colorAttachment.initialLayout = FlagsToInitialImageLayout(flags);
        colorAttachment.loadOp = FlagsToLoadOp(flags);

        attachments.push_back(colorAttachment);

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

            colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.initialLayout = colorAttachment.initialLayout;
            colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;            

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

        output.image.resize(1); output.imageView.resize(output.image.size());

        for (size_t i = 0; i < output.image.size(); ++i)
        {
            output.image[i] = CreateRef<Image>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageFormat(), VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
            output.imageView[i] = CreateRef<ImageView>(*output.image[i], output.image[i]->GetVkFormat());
        }

        renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), output.imageView, pipeline->GetRenderPass(), samples, flags & RendererFlags_UseDepth);

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newSize)
        {
            vkQueueWaitIdle(Queues::graphicsQueue);
            for (size_t i = 0; i < output.image.size(); ++i)
            {
                output.image[i].reset();
                output.image[i] = CreateRef<Image>(newSize, ctx.GetSwapChain().GetImageFormat(), VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

                output.imageView[i].reset();
                output.imageView[i] = CreateRef<ImageView>(*output.image[i], output.image[i]->GetVkFormat());
            }

            renderTarget.reset();
            renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), output.imageView, pipeline->GetRenderPass(), this->samples, this->flags & RendererFlags_UseDepth);
            RecordAll();
        });
    }

    void Renderer3D::Record(const size_t cmdIndex)
    {
        CommandPool& pool = GetCommandPool(cmdIndex);
        CommandBuffer& cmd = GetCommandBuffer(cmdIndex);
        const Framebuffer& framebuffer = renderTarget->GetFramebuffer();

        pool.Reset();
        cmd.Begin();

            if (needsResize[cmdIndex])
            {
                cmd.SetViewport(newSize.x, newSize.y);
                cmd.SetScissors(newSize.x, newSize.y);
                needsResize[cmdIndex] = false;
            }

            cmd.BeginRenderPass(pipeline->GetRenderPass(), framebuffer, {0.1f, 0.4f, 0.9f, 1.0f});
                cmd.BindPipeline(*pipeline);
                    for (size_t i = 0; i < renderable.size(); ++i)
                    {
                        const uint32_t dynamicOffset {static_cast<uint32_t>(i * DynamicAlignment<EntityUBO>::Get())};
                        cmd.BindVertexBuffers({renderable[i]->GetVertexBuffer().UnderlyingPtr()}, {0});
                            cmd.BindIndexBuffer(renderable[i]->GetIndexBuffer().UnderlyingRef());
                                cmd.BindDescriptorSets(*pipeline, 1, &renderable[i]->GetDescriptorSet().GetVkDescriptorSet(), 1, &dynamicOffset);
                                    cmd.DrawIndexed(renderable[i]->GetNumIndices(), 1, 0, 0, 0);
                    }
            cmd.EndRenderPass();
        cmd.End();
    }

    EntityUniformBuffer<EntityUBO>& Renderer3D::GetEntityUBO()
    {
        return *entityUniformBuffer;
    }

    SceneUniformBuffer<SceneUBO>& Renderer3D::GetSceneUBO()
    {
        return *sceneUniformBuffer;
    }

    size_t Renderer3D::GetNumRenderableEntities() const
    {
        return renderable.size();
    }

    SpaceObject& Renderer3D::GetSpaceObject(const size_t i)
    {
        return renderable[i]->GetSpaceObject();
    }

    PerspectiveSpace& Renderer3D::GetPerspectiveSpace()
    {
        return *perspectiveSpace;
    }
}