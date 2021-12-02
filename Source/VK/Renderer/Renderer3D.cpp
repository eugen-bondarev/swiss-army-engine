#include "Renderer3D.h"

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
    ) : Renderer(vertexShaderCode, fragmentShaderCode, numCmdBuffers, samples, useDepth, isOutput, ctx)
    {
        needsResize.resize(GetNumCmdBuffers());

        for (size_t i = 0; i < needsResize.size(); ++i) needsResize[i] = true;
        newSize = ctx.GetWindow().GetSize();

        ctx.GetWindow().ResizeSubscribe([&](const Vec2ui newViewportSize)
        {
        for (size_t i = 0; i < needsResize.size(); ++i) needsResize[i] = true;
            newSize = newViewportSize;
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
                    for (size_t j = 0; j < renderable.size(); ++j)
                    {
                        const uint32_t dynamicOffset {static_cast<uint32_t>(j * DynamicAlignment<VK::EntityUBO>::Get())};
                        cmd.BindVertexBuffers({renderable[j]->GetVertexBuffer().UnderlyingPtr()}, {0});
                            cmd.BindIndexBuffer(renderable[j]->GetIndexBuffer().UnderlyingRef());
                                cmd.BindDescriptorSets(*pipeline, 1, &renderable[j]->GetDescriptorSet().GetVkDescriptorSet(), 1, &dynamicOffset);
                                    cmd.DrawIndexed(renderable[j]->GetNumIndices(), 1, 0, 0, 0);
                    }
            cmd.EndRenderPass();
        cmd.End();
    }
}