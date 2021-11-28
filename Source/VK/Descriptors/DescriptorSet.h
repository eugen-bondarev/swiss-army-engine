#ifndef __VK_Descriptors_DescriptorSet_h__
#define __VK_Descriptors_DescriptorSet_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(DescriptorPool);
    FORWARD_DECLARE(Device);
    
    class DescriptorSet
    {
    public:
        DescriptorSet(const DescriptorPool& descriptorPool, const std::vector<VkDescriptorSetLayout>& layouts, const Device* device = nullptr);
       ~DescriptorSet();

        void Update(const std::vector<VkWriteDescriptorSet>& writeDescriptorSets);

        const VkDescriptorSet& GetVkDescriptorSet() const;

    private:
        const Device& device;
        VkDescriptorSet vkDescriptorSet;

        DescriptorSet(const DescriptorSet&) = delete;
        DescriptorSet& operator=(const DescriptorSet&) = delete;
    };

    VkWriteDescriptorSet CreateWriteDescriptorSet(const DescriptorSet* descriptorSet, const uint32_t binding, const VkDescriptorType descriptorType, const VkDescriptorBufferInfo* descriptorBufferInfo);
    VkWriteDescriptorSet CreateWriteDescriptorSet(const DescriptorSet* descriptorSet, const uint32_t binding, const VkDescriptorType descriptorType, const VkDescriptorImageInfo* descriptorImageInfo);
}

#endif