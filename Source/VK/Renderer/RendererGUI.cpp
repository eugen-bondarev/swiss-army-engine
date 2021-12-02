#include "RendererGUI.h"

#include <imgui_impl_vulkan.h>

namespace VK
{
    RendererGUI::RendererGUI(
        const Str& vertexShaderCode, 
        const Str& fragmentShaderCode, 
        const size_t numCmdBuffers, 
        const size_t samples, 
        const bool useDepth, 
        const bool isOutput,
        GraphicsContext& ctx
    ) : Renderer(vertexShaderCode, fragmentShaderCode, numCmdBuffers, samples, useDepth, isOutput, ctx)
    {
        needsResize.resize(GetNumCmdBuffers(), true);
        newSize = ctx.GetWindow().GetSize();

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newViewportSize)
        {
            needsResize.resize(GetNumCmdBuffers(), true);
            newSize = newViewportSize;
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
                VkViewport viewport{};
                viewport.x = 0.0f;
                viewport.y = 0.0f;
                viewport.width = newSize.x;
                viewport.height = newSize.y;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;

                VkRect2D scissor{};
                scissor.offset = {0, 0};
                scissor.extent = {static_cast<uint32_t>(newSize.x), static_cast<uint32_t>(newSize.y)};

                vkCmdSetViewport(cmd.GetVkCommandBuffer(), 0, 1, &viewport);
                vkCmdSetScissor(cmd.GetVkCommandBuffer(), 0, 1, &scissor);
                needsResize[cmdIndex] = false;
            }

            cmd.BeginRenderPass(pipeline->GetRenderPass(), framebuffer);
                cmd.BindPipeline(*pipeline);
                    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.GetVkCommandBuffer());
            cmd.EndRenderPass();
        cmd.End(); 
    }
}