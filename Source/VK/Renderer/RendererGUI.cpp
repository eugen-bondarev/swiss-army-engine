#include "RendererGUI.h"

#include "../SwapChain/SwapChain.h"
#include "../Device/QueueFamily.h"
#include <imgui_impl_vulkan.h>
#include "../Device/Device.h"
#include <imgui_impl_glfw.h>

namespace VK
{
    RendererGUI::RendererGUI(
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
            samples,
            useDepth,
            isOutput
        );

        ctx.GetWindow().BeginFrameSubscribe([&]()
        {
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
                ImGui::ShowDemoWindow();
            ImGui::Render();
        });

        ctx.GetWindow().EndFrameSubscribe([&]()
        {
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        });
    }

    void RendererGUI::CreateGraphicsResources(const size_t samples, const bool useDepth, const bool isOutput)
    {
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

        renderPass = CreatePtr<RenderPass>(
            attachments, 
            samples, 
            useDepth, 
            ctx.GetDevice()
        );

        renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), *renderPass, samples, useDepth);

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newSize)
        {
            space->SetAspectRatio(newSize.x / newSize.y);
            vkQueueWaitIdle(Queues::graphicsQueue);
            renderTarget.reset();
            renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), *renderPass, samples, useDepth);
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

            cmd.BeginRenderPass(*renderPass, framebuffer);
                ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.GetVkCommandBuffer());
            cmd.EndRenderPass();
        cmd.End(); 
    }

    RenderPass& RendererGUI::GetRenderPass()
    {
        return *renderPass;
    }
}