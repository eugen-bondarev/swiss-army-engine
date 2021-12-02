#ifndef __VK_Renderer_RendererGUI_h__
#define __VK_Renderer_RendererGUI_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"
#include "Renderer.h"

namespace VK
{
    class RendererGUI : public Renderer
    {
    public:
        RendererGUI(
            const Str& vertexShaderCode,
            const Str& fragmentShaderCode,
            const size_t numCmdBuffers,
            const size_t samples,
            const bool useDepth,
            const bool isOutput,
            GraphicsContext& ctx = GetCurrentGraphicsContext()
        );

        void Record(const size_t cmdIndex) override;

    private:
        Vec<bool> needsResize;
        Vec2ui newSize;
    };
}

#endif