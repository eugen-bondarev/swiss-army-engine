#include "DescriptorSet.h"

#include "../../GraphicsContext.h"

namespace VK
{
    
    VkWriteDescriptorSet CreateWriteDescriptorSet(DescriptorSet* descriptor_set, uint32_t binding, VkDescriptorType descriptor_type, const VkDescriptorImageInfo* descriptor_image_info)
    {			
        VkWriteDescriptorSet writeDesc = {};
        writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDesc.dstSet = descriptor_set->GetVkDescriptorSet();
        writeDesc.dstBinding = binding;
        writeDesc.dstArrayElement = 0;
        writeDesc.descriptorType = descriptor_type;
        writeDesc.descriptorCount = 1;
        writeDesc.pImageInfo = descriptor_image_info;
        return writeDesc;
    }

    VkDescriptorSet& DescriptorSet::GetVkDescriptorSet()
    {
        return vkDescriptorSet;
    }

    DescriptorSet::DescriptorSet(const DescriptorPool* descriptorPool, const std::vector<VkDescriptorSetLayout>& layouts, const Device* device) : device{device ? *device : *GetDevice()}
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool->GetVkDescriptorPool();
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layouts.data();
        VK_TRY(vkAllocateDescriptorSets(this->device.GetVkDevice(), &allocInfo, &vkDescriptorSet));
    }

    DescriptorSet::~DescriptorSet()
    {
        
    }
}