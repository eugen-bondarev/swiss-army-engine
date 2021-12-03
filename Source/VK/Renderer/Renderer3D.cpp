#include "Renderer3D.h"

#include "../SwapChain/SwapChain.h"
#include "../Device/QueueFamily.h"
#include <imgui_impl_vulkan.h>
#include "../Device/Device.h"

namespace VK
{
    Renderer3D::Renderer3D(
        const Str& vertexShaderCode, 
        const Str& fragmentShaderCode, 
        const size_t numCmdBuffers, 
        const size_t samples, 
        const bool useDepth, 
        const bool isOutput,
        GraphicsContext& ctx
    ) : Renderer(numCmdBuffers, samples, useDepth, isOutput, ctx)
    {
        needsResize.resize(GetNumCmdBuffers());

        for (size_t i = 0; i < needsResize.size(); ++i) needsResize[i] = true;
        newSize = ctx.GetWindow().GetSize();

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newViewportSize)
        {
            for (size_t i = 0; i < needsResize.size(); ++i) needsResize[i] = true;
            newSize = newViewportSize;
        });

        CreateGraphicsResources(
            vertexShaderCode,
            fragmentShaderCode,
            samples,
            useDepth,
            isOutput
        );
    }

    void Renderer3D::CreateGraphicsResources(const Str& vertexShaderCode, const Str& fragmentShaderCode, const size_t samples, const bool useDepth, const bool isOutput)
    {
        const Vec<VkDescriptorSetLayoutBinding> bindings({
            CreateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER),
            CreateBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC),
            CreateBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        });

        descriptorSetLayout = CreatePtr<DescriptorSetLayout>(bindings);
		const BindingDescriptions bindingDescriptors {Vertex::GetBindingDescriptions()};
		const AttributeDescriptions attributeDescriptors {Vertex::GetAttributeDescriptions()};

        AttachmentDescriptions attachments;
        VkAttachmentDescription swapChainAttachment = GetSwapChain().GetDefaultAttachmentDescription(SamplesToVKFlags(samples));
        if (isOutput)
        {
            swapChainAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            swapChainAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            swapChainAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        }
        else
        {
            swapChainAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            swapChainAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        attachments.push_back(swapChainAttachment);

        if (useDepth)
        {
            const VkAttachmentDescription depthAttachment = Util::CreateDefaultDepthAttachment(ctx.GetDevice().FindDepthFormat(), SamplesToVKFlags(samples));
            attachments.push_back(depthAttachment);
        }

        if (samples > 1)
        {
            VkAttachmentDescription colorAttachmentResolve{};
            colorAttachmentResolve.format = GetSwapChain().GetImageFormat();
            colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachmentResolve.finalLayout = isOutput ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
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
            useDepth,
            ctx.GetDevice()
        );

        renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), pipeline->GetRenderPass(), samples, useDepth);

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newSize)
        {
            space->SetAspectRatio(newSize.x / newSize.y);
            vkQueueWaitIdle(Queues::graphicsQueue);
            renderTarget.reset();
            renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), pipeline->GetRenderPass(), samples, useDepth);
        });
    }

    void Renderer3D::Record(const size_t cmdIndex)
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
}