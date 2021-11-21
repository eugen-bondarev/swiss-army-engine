#ifndef __VK_Entities_Descriptors_DescriptorSet_h__
#define __VK_Entities_Descriptors_DescriptorSet_h__

#pragma once

#include "../Device/Device.h"
#include "DescriptorPool.h"
#include "../../Common.h"

namespace VK
{
    class DescriptorSet
    {
    public:
        DescriptorSet(const DescriptorPool* descriptorPool, const std::vector<VkDescriptorSetLayout>& layouts, const Device* device = nullptr);
       ~DescriptorSet();

        VkDescriptorSet& GetVkDescriptorSet();

    private:
        const Device& device;
        VkDescriptorSet vkDescriptorSet;

        DescriptorSet(const DescriptorSet&) = delete;
        DescriptorSet& operator=(const DescriptorSet&) = delete;
    };

    VkWriteDescriptorSet CreateWriteDescriptorSet(DescriptorSet* descriptor_set, uint32_t binding, VkDescriptorType descriptor_type, const VkDescriptorImageInfo* descriptor_image_info);
}

#endif