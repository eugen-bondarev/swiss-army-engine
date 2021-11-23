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

        void Update(const std::vector<VkWriteDescriptorSet>& write_descriptor_sets);

        VkDescriptorSet& GetVkDescriptorSet();

    private:
        const Device& device;
        VkDescriptorSet vkDescriptorSet;

        DescriptorSet(const DescriptorSet&) = delete;
        DescriptorSet& operator=(const DescriptorSet&) = delete;
    };

    VkWriteDescriptorSet CreateWriteDescriptorSet(DescriptorSet* descriptor_set, uint32_t binding, VkDescriptorType descriptor_type, const VkDescriptorBufferInfo* descriptor_buffer_info);
    VkWriteDescriptorSet CreateWriteDescriptorSet(DescriptorSet* descriptor_set, uint32_t binding, VkDescriptorType descriptor_type, const VkDescriptorImageInfo* descriptor_image_info);
}

#endif