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
        RendererImGui(const RendererFlags flags, GraphicsContext& ctx = GetCurrentGraphicsContext());
       ~RendererImGui();

        void InFrame() override;
        void Record(const size_t cmdIndex) override;

        RenderPass& GetRenderPass();

    private:
        void CreateGraphicsResources(const RendererFlags flags);

        Ptr<RenderPass> renderPass;
        
        std::vector<bool> needsResize;
        Vec2ui newSize;
    };
}

#endif