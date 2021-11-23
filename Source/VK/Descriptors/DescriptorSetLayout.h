#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    
    VkDescriptorSetLayoutBinding CreateBinding(uint32_t index, VkDescriptorType type, VkShaderStageFlags stage_flags = VK_SHADER_STAGE_VERTEX_BIT);

    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const Device* device = nullptr);
        ~DescriptorSetLayout();

        VkDescriptorSetLayout& GetVkDescriptorSetLayout();

    private:
        const Device& device;
        VkDescriptorSetLayout vkDescriptorSetLayout;

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
    };
}