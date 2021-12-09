#ifndef __VK_Renderer_Renderer_h__
#define __VK_Renderer_Renderer_h__

#pragma once

#include "../Descriptors/DescriptorSetLayout.h"
#include "../Logic/Scene/PerspectiveSpace.h"
#include "../Logic/Scene/OrthogonalSpace.h"
#include "../Memory/EntityUniformBuffer.h"
#include "../Memory/SceneUniformBuffer.h"
#include "../RenderTarget/RenderTarget.h"
#include "../Commands/CommandBuffer.h"
#include "../Commands/CommandPool.h"
#include "../Pipeline/Pipeline.h"
#include "../Image/Texture2D.h"
#include "../GraphicsContext.h"
#include "../../Util/Assets.h"
#include "../Frame/Frame.h"
#include "IRenderable.h"
#include "../Common.h"

using Flag = unsigned int;
using RendererFlags = Flag;

enum RendererFlags_
{
    RendererFlags_None      = 1 << 0,
    RendererFlags_Load      = 1 << 1,
    RendererFlags_Clear     = 1 << 2,
    RendererFlags_Output    = 1 << 3,
    RendererFlags_UseDepth  = 1 << 4,
    RendererFlags_Offscreen = 1 << 5,
};

namespace VK
{
    class Renderer
    {
    public:
        Renderer(const size_t numCmdBuffers, const size_t samples, const RendererFlags flags, GraphicsContext& graphicsContext = GetCurrentGraphicsContext());

        virtual void Record(const size_t cmdIndex) = 0;
        void RecordAll();

        void Render(const Frame& frame, const uint32_t swapChainImageIndex, const bool resetFence, const uint32_t waitSemaphoreIndex = 0, const uint32_t signalSemaphoreIndex = 1);

        CommandBuffer& GetCommandBuffer(const size_t i);
        CommandPool& GetCommandPool(const size_t i);
        size_t GetNumCmdBuffers() const;
        size_t GetNumCmdPool() const;

        const DescriptorSetLayout& GetDescriptorSetLayout() const;

    protected:
        GraphicsContext& ctx;

        std::vector<Ptr<CommandPool>> commandPools;
        std::vector<Ptr<CommandBuffer>> commandBuffers;
        void CreateCmdEntities(const size_t numCmdBuffers);
        
        Ptr<DescriptorSetLayout> descriptorSetLayout;
        Ptr<RenderTarget> renderTarget;

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
    };
}

#endif