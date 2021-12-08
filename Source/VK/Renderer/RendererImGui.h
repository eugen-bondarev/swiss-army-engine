#ifndef __VK_Renderer_RendererImGui_h__
#define __VK_Renderer_RendererImGui_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"
#include "Renderer.h"

namespace VK
{
    class RendererImGui : public Renderer
    {
    public:
        RendererImGui(
            const size_t numCmdBuffers,
            const size_t samples,
            const RendererFlags flags,
            GraphicsContext& ctx = GetCurrentGraphicsContext()
        );

        void Record(const size_t cmdIndex) override;

        RenderPass& GetRenderPass();

    private:
        void CreateGraphicsResources(
            const size_t samples, 
            const RendererFlags flags
        );

        Ptr<RenderPass> renderPass;
        
        std::vector<bool> needsResize;
        Vec2ui newSize;
    };
}

#endif