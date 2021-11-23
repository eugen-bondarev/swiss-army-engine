#pragma once

#include "../Common.h"
#include "../Objects.h"

namespace VK
{
    VkDescriptorSetLayoutBinding CreateBinding(uint32_t index, VkDescriptorType type, VkShaderStageFlags stage_flags = VK_SHADER_STAGE_VERTEX_BIT);

    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const Global::Device* device = nullptr);
        ~DescriptorSetLayout();

        VkDescriptorSetLayout& GetVkDescriptorSetLayout();

    private:
        const Global::Device& device;
        VkDescriptorSetLayout vkDescriptorSetLayout;

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
    };
}