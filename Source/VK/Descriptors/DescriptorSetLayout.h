#ifndef __VK_Descriptors_DescriptorSetLayout_h__
#define __VK_Descriptors_DescriptorSetLayout_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    
    VkDescriptorSetLayoutBinding CreateBinding(uint32_t index, VkDescriptorType type, VkShaderStageFlags stageFlags = VK_SHADER_STAGE_VERTEX_BIT);

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