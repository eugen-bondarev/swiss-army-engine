#ifndef __VK_Renderer_Renderer_h__
#define __VK_Renderer_Renderer_h__

#pragma once

#include "../Descriptors/DescriptorSetLayout.h"
#include "../Memory/EntityUniformBuffer.h"
#include "../Memory/SceneUniformBuffer.h"
#include "../Commands/CommandBuffer.h"
#include "../Commands/CommandPool.h"
#include "../Pipeline/Pipeline.h"
#include "../Image/Texture2D.h"
#include "../../Util/Assets.h"
#include "IRenderable.h"
#include "../Common.h"

namespace VK
{
    class Renderer
    {
    public:
        Renderer(const Str& vertexShaderCode, const Str& fragmentShaderCode, const size_t numCmdBuffers, const Texture2D& depthTexture);

        SpaceObject& Add(const ::Util::ModelAsset& modelAsset, const ::Util::ImageAsset& imageAsset);

        void Record();
        void Render();

        CommandBuffer& GetCommandBuffer(const size_t i);
        CommandPool& GetCommandPool(const size_t i);
        size_t GetNumCmdBuffers() const;
        size_t GetNumCmdPool() const;

        const Pipeline& GetPipeline() const;
        const DescriptorSetLayout& GetDescriptorSetLayout() const;

        EntityUniformBuffer<EntityUBO>& GetEntityUBO();
        SceneUniformBuffer<SceneUBO>& GetSceneUBO();

        size_t GetNumRenderableEntities() const;
        SpaceObject& GetSpaceObject(const size_t i);

    private:
        Vec<Ptr<IRenderable>> renderable;

        Ptr<EntityUniformBuffer<EntityUBO>> entityUniformBuffer;
        Ptr<SceneUniformBuffer<SceneUBO>> sceneUniformBuffer;
        void CreateUniformBuffers();

        Vec<Ptr<CommandPool>> commandPools;
        Vec<Ptr<CommandBuffer>> commandBuffers;
        void CreateCmdEntities(const size_t numCmdBuffers);

        Ptr<Pipeline> pipeline;
        Ptr<DescriptorSetLayout> descriptorSetLayout;
        void CreatePipeline(const Str& vertexShaderCode, const Str& fragmentShaderCode);

        const Texture2D& depthTexture;

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
    };
}

#endif