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
    RendererFlags_Output    = 1 << 2,
};

namespace VK
{
    class Renderer
    {
    public:
        Renderer(const size_t numCmdBuffers, const size_t samples, const bool useDepth, const bool isOutput, const RendererFlags flags, GraphicsContext& graphicsContext = GetCurrentGraphicsContext());

        SpaceObject& Add(const ::Util::ModelAsset& modelAsset, const ::Util::ImageAsset& imageAsset);

        virtual void Record(const size_t cmdIndex) = 0;
        void RecordAll();

        void UpdateUniformBuffers(const float ratio);
        void Render(const Frame& frame, const uint32_t swapChainImageIndex, const bool resetFence, const uint32_t waitSemaphoreIndex = 0, const uint32_t signalSemaphoreIndex = 1);

        CommandBuffer& GetCommandBuffer(const size_t i);
        CommandPool& GetCommandPool(const size_t i);
        size_t GetNumCmdBuffers() const;
        size_t GetNumCmdPool() const;

        const DescriptorSetLayout& GetDescriptorSetLayout() const;

        EntityUniformBuffer<EntityUBO>& GetEntityUBO();
        SceneUniformBuffer<SceneUBO>& GetSceneUBO();

        size_t GetNumRenderableEntities() const;
        SpaceObject& GetSpaceObject(const size_t i);

        PerspectiveSpace& GetPerspectiveSpace();
        OrthogonalSpace& GetOrthogonalSpace();

    protected:
        GraphicsContext& ctx;

        Vec<Ptr<IRenderable>> renderable;

        Ptr<EntityUniformBuffer<EntityUBO>> entityUniformBuffer;
        Ptr<SceneUniformBuffer<SceneUBO>> sceneUniformBuffer;
        Ptr<PerspectiveSpace> perspectiveSpace;
        Ptr<OrthogonalSpace> orthogonalSpace;
        void CreateUniformBuffers();

        Vec<Ptr<CommandPool>> commandPools;
        Vec<Ptr<CommandBuffer>> commandBuffers;
        void CreateCmdEntities(const size_t numCmdBuffers);
        
        Ptr<DescriptorSetLayout> descriptorSetLayout;
        Ptr<RenderTarget> renderTarget;

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
    };
}

#endif