#ifndef __VK_Entities_Descriptors_DescriptorSetLayout_h__
#define __VK_Entities_Descriptors_DescriptorSetLayout_h__

#pragma once

#include "../Device/Device.h"
#include "../../Common.h"

namespace VK
{    
    VkDescriptorSetLayoutBinding CreateBinding(uint32_t index, VkDescriptorType type, VkShaderStageFlags stageFlags);

    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const Device* device = nullptr);
       ~DescriptorSetLayout();

        const VkDescriptorSetLayout& GetVkDescriptorSetLayout() const;

    private:
        const Device& device;

        VkDescriptorSetLayout vkDescriptorSetLayout;

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
    };
}

#endif