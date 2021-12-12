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
    template <typename T>
    class IRenderable
    {
    public:
        IRenderable(
            const ::Util::ModelAsset<T>& modelAsset, 
            const ::Util::ImageAsset& imageAsset, 
            const SceneUniformBuffer<SceneUBO>& sceneUniformBuffer, 
                EntityUniformBuffer<EntityUBO>& entityUniformBuffer, 
            const DescriptorSetLayout& descriptorSetLayout,
            const size_t index
        )
        {
            vertexBuffer = CreatePtr<VertexBuffer>(modelAsset.vertices);
            indexBuffer = CreatePtr<IndexBuffer>(modelAsset.indices);
            texture = CreatePtr<Texture2D>(
                imageAsset.size, 
                4, 
                imageAsset.data, 
                imageAsset.mipLevels, 
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
            );
            numIndices = modelAsset.indices.size();

            descriptorSet = CreatePtr<DescriptorSet>(
                GetDefaultDescriptorPool(),
                std::vector<VkDescriptorSetLayout> {descriptorSetLayout.GetVkDescriptorSetLayout()}
            );

            descriptorSet->Update({
                CreateWriteDescriptorSet(descriptorSet.get(), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &sceneUniformBuffer.GetVkDescriptor()),
                CreateWriteDescriptorSet(descriptorSet.get(), 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &entityUniformBuffer.GetVkDescriptor()),
                CreateWriteDescriptorSet(descriptorSet.get(), 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &texture->GetImageView().GetVkDescriptor())
            });

            spaceObject = CreatePtr<SpaceObject>(&entityUniformBuffer()[index]);
        }

        SpaceObject& GetSpaceObject()
        {
            return *spaceObject;
        }

        VertexBuffer& GetVertexBuffer()
        {
            return *vertexBuffer;
        }

        IndexBuffer& GetIndexBuffer()
        {
            return *indexBuffer;
        }

        DescriptorSet& GetDescriptorSet()
        {
            return *descriptorSet;
        }

        unsigned int GetNumIndices() const
        {
            return numIndices;
        }

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