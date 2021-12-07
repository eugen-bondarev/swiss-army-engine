#ifndef __VK_Renderer_Renderer3D_h__
#define __VK_Renderer_Renderer3D_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"
#include "Renderer.h"

namespace VK
{
    class Renderer3D : public Renderer
    {
    public:
        Renderer3D(
            const std::string& vertexShaderCode,
            const std::string& fragmentShaderCode,
            const size_t numCmdBuffers,
            const size_t samples,
            const RendererFlags flags,
            GraphicsContext& ctx = GetCurrentGraphicsContext()
        );

        void Record(const size_t cmdIndex) override;

    private:
        void CreateGraphicsResources(
            const std::string& vertexShaderCode, 
            const std::string& fragmentShaderCode, 
            const size_t samples, 
            const RendererFlags flags
        );

        Ptr<Pipeline> pipeline;

        std::vector<bool> needsResize;
        Vec2ui newSize;
    };
}

#endif