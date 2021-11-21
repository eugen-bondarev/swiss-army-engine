#include "DescriptorSetLayout.h"

#include "../../GraphicsContext.h"

namespace VK
{
    VkDescriptorSetLayoutBinding CreateBinding(uint32_t index, VkDescriptorType type, VkShaderStageFlags stage_flags)
    {
        VkDescriptorSetLayoutBinding binding{};		
        binding.binding = index;
        binding.descriptorType = type;
        binding.descriptorCount = 1;
        binding.stageFlags = stage_flags;
        binding.pImmutableSamplers = nullptr; // Optional

        return binding;
    }

    DescriptorSetLayout::DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const Device* device) : device{device ? *device : *GetDevice()}
    {
        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        createInfo.pBindings = bindings.data();

        VK_TRY(vkCreateDescriptorSetLayout(this->device.GetVkDevice(), &createInfo, nullptr, &vkDescriptorSetLayout));
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(device.GetVkDevice(), vkDescriptorSetLayout, nullptr);
    }

    const VkDescriptorSetLayout& DescriptorSetLayout::GetVkDescriptorSetLayout() const
    {
        return vkDescriptorSetLayout;
    }
}