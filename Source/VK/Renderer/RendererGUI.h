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
            const std::string& vsCode,
            const std::string& fsCode,
            const size_t samples,
            const RendererFlags flags,
            GraphicsContext& ctx = GetCurrentGraphicsContext()
        );

        void Record(const size_t cmdIndex) override;
        
        SpaceObject& Add(const ::Util::ModelAsset<PredefinedVertexLayouts::Vertex2D>& modelAsset, const ::Util::ImageAsset& imageAsset);

        void UpdateUniformBuffers();
        OrthogonalSpace& GetOrthogonalSpace();

        EntityUniformBuffer<EntityUBO>& GetEntityUBO();
        SceneUniformBuffer<SceneUBO>& GetSceneUBO();

        size_t GetNumRenderableEntities() const;
        SpaceObject& GetSpaceObject(const size_t i);
        
        std::vector<Ptr<IRenderable<PredefinedVertexLayouts::Vertex2D>>> renderable;

    private:
        void CreateGraphicsResources(
            const std::string& vertexShaderCode, 
            const std::string& fragmentShaderCode
        );

        Ptr<Pipeline> pipeline;

        Ptr<EntityUniformBuffer<EntityUBO>> entityUniformBuffer;
        Ptr<SceneUniformBuffer<SceneUBO>> sceneUniformBuffer;
        Ptr<OrthogonalSpace> orthogonalSpace;
        void CreateUniformBuffers();

        std::vector<bool> needsResize;
        Vec2ui newSize;
    };
}

#endif