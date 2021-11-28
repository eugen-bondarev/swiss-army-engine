#include "IRenderable.h"

namespace VK
{    
    IRenderable::IRenderable(
        const ::Util::ModelAsset& modelAsset, 
        const ::Util::ImageAsset& imageAsset, 
        const SceneUniformBuffer<SceneUBO>& sceneUniformBuffer, 
              EntityUniformBuffer<EntityUBO>& entityUniformBuffer, 
        const DescriptorSetLayout& descriptorSetLayout,
        const size_t index
    )
    {
        vertexBuffer = CreatePtr<VertexBuffer>(modelAsset.vertices);
        indexBuffer = CreatePtr<IndexBuffer>(modelAsset.indices);
        texture = CreatePtr<Texture2D>(imageAsset.size, 4, imageAsset.data);
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

    SpaceObject& IRenderable::GetSpaceObject()
    {
        return *spaceObject;
    }

    VertexBuffer& IRenderable::GetVertexBuffer()
    {
        return *vertexBuffer;
    }

    IndexBuffer& IRenderable::GetIndexBuffer()
    {
        return *indexBuffer;
    }

    DescriptorSet& IRenderable::GetDescriptorSet()
    {
        return *descriptorSet;
    }

    unsigned int IRenderable::GetNumIndices() const
    {
        return numIndices;
    }
}