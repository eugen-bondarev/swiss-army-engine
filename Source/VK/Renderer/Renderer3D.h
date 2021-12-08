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
        
        SpaceObject& Add(const ::Util::ModelAsset<::Vertex>& modelAsset, const ::Util::ImageAsset& imageAsset);

        void UpdateUniformBuffers(const float ratio);
        PerspectiveSpace& GetPerspectiveSpace();

        EntityUniformBuffer<EntityUBO>& GetEntityUBO();
        SceneUniformBuffer<SceneUBO>& GetSceneUBO();

        size_t GetNumRenderableEntities() const;
        SpaceObject& GetSpaceObject(const size_t i);

    private:
        void CreateGraphicsResources(
            const std::string& vertexShaderCode, 
            const std::string& fragmentShaderCode, 
            const size_t samples, 
            const RendererFlags flags
        );

        Ptr<Pipeline> pipeline;

        Ptr<EntityUniformBuffer<EntityUBO>> entityUniformBuffer;
        Ptr<SceneUniformBuffer<SceneUBO>> sceneUniformBuffer;
        std::vector<Ptr<IRenderable<::Vertex>>> renderable;
        Ptr<PerspectiveSpace> perspectiveSpace;
        void CreateUniformBuffers();

        std::vector<bool> needsResize;
        Vec2ui newSize;
    };
}

#endif