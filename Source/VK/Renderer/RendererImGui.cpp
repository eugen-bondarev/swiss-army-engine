#include "RendererImGui.h"

#include "../SwapChain/SwapChain.h"
#include "../Device/QueueFamily.h"
#include <imgui_impl_vulkan.h>
#include "../Device/Device.h"
#include <imgui_impl_glfw.h>

namespace VK
{
    RendererImGui::RendererImGui(
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

        CreateGraphicsResources(
            samples,
            flags
        );

        // ctx.GetWindow().BeginFrameSubscribe([&]()
        // {
        //     ImGui_ImplVulkan_NewFrame();
        //     ImGui_ImplGlfw_NewFrame();
        //     ImGui::NewFrame();
        //         ImGui::ShowDemoWindow();
        //     ImGui::Render();
        // });

        ctx.GetWindow().EndFrameSubscribe([&]()
        {
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        });
    }

    void RendererImGui::CreateGraphicsResources(const size_t samples, const RendererFlags flags)
    {
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

        renderPass = CreatePtr<RenderPass>(
            attachments,
            samples,
            false,
            ctx.GetDevice()
        );

        renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), *renderPass, samples, false);

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newSize)
        {
            // orthogonalSpace->SetAspectRatio(newSize.x / newSize.y);
            vkQueueWaitIdle(Queues::graphicsQueue);
            renderTarget.reset();
            renderTarget = CreatePtr<RenderTarget>(ctx.GetSwapChain().GetSize(), ctx.GetSwapChain().GetImageViews(), *renderPass, samples, false);
        });
    }

    void RendererImGui::Record(const size_t cmdIndex)
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

    RenderPass& RendererImGui::GetRenderPass()
    {
        return *renderPass;
    }
}