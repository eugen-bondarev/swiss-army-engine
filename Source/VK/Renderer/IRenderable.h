#ifndef __VK_Renderer_IRenderable_h__
#define __VK_Renderer_IRenderable_h__

#include "../Descriptors/DescriptorSetLayout.h"
#include "../Pipeline/VertexLayouts/Layouts.h"
#include "../Memory/EntityUniformBuffer.h"
#include "../Memory/SceneUniformBuffer.h"
#include "../Descriptors/DescriptorSet.h"
#include "../Logic/Scene/SpaceObject.h"
#include "../Logic/Scene/EntityUBO.h"
#include "../Logic/Scene/SceneUBO.h"
#include "../Memory/VertexBuffer.h"
#include "../Memory/IndexBuffer.h"
#include "../Image/Texture2D.h"
#include "../../Util/Assets.h"
#include "../Common.h"

namespace VK
{
    class IRenderable
    {
    public:
        IRenderable(
            const ::Util::ModelAsset& modelAsset, 
            const ::Util::ImageAsset& imageAsset, 
            const SceneUniformBuffer<SceneUBO>& sceneUniformBuffer, 
                  EntityUniformBuffer<EntityUBO>& entityUniformBuffer, 
            const DescriptorSetLayout& descriptorSetLayout,
            const size_t index
        );

        SpaceObject& GetSpaceObject();
        VertexBuffer& GetVertexBuffer();
        IndexBuffer& GetIndexBuffer();
        DescriptorSet& GetDescriptorSet();
        unsigned int GetNumIndices() const;

    private:
        Ptr<SpaceObject> spaceObject;
        Ptr<DescriptorSet> descriptorSet;
        Ptr<VertexBuffer> vertexBuffer;
        Ptr<IndexBuffer> indexBuffer;
        Ptr<Texture2D> texture;
        unsigned int numIndices;
    };
}

#endif