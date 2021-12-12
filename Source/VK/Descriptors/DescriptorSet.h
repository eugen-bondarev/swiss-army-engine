#ifndef __VK_Descriptors_DescriptorSet_h__
#define __VK_Descriptors_DescriptorSet_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(DescriptorPool);
    FORWARD_DECLARE(DescriptorSet);
    FORWARD_DECLARE(Device);

    VkWriteDescriptorSet CreateWriteDescriptorSet(const DescriptorSet* descriptorSet, const uint32_t binding, const VkDescriptorType descriptorType, const VkDescriptorBufferInfo* descriptorBufferInfo);
    VkWriteDescriptorSet CreateWriteDescriptorSet(const DescriptorSet* descriptorSet, const uint32_t binding, const VkDescriptorType descriptorType, const VkDescriptorImageInfo* descriptorImageInfo);
    
    class DescriptorSet
    {
    public:
        DescriptorSet(const DescriptorPool& descriptorPool, const std::vector<VkDescriptorSetLayout>& layouts, const Device* device = nullptr);
       ~DescriptorSet();

        void Update(const std::vector<VkWriteDescriptorSet>& writeDescriptorSets);

        void SetBinding(const uint32_t binding, const VkDescriptorImageInfo imageInfo, const VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
        {
            Update({CreateWriteDescriptorSet(this, binding, descriptorType, &imageInfo)});
        }

        void SetBinding(const uint32_t binding, const VkDescriptorBufferInfo bufferInfo, const VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
        {
            Update({CreateWriteDescriptorSet(this, binding, descriptorType, &bufferInfo)});
        }

        const VkDescriptorSet& GetVkDescriptorSet() const;

    private:
        const Device& device;
        VkDescriptorSet vkDescriptorSet;

        DescriptorSet(const DescriptorSet&) = delete;
        DescriptorSet& operator=(const DescriptorSet&) = delete;
    };
}

#endif