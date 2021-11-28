#include "DescriptorSet.h"

#include "../GraphicsContext.h"
#include "../Device/Device.h"
#include "DescriptorPool.h"

namespace VK
{
    VkWriteDescriptorSet CreateWriteDescriptorSet(const DescriptorSet* descriptor_set, const uint32_t binding, const VkDescriptorType descriptor_type, const VkDescriptorBufferInfo* descriptor_buffer_info)
    {
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptor_set->GetVkDescriptorSet();
        descriptorWrite.dstBinding = binding;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = descriptor_type;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = descriptor_buffer_info;
        return descriptorWrite;
    }

    VkWriteDescriptorSet CreateWriteDescriptorSet(const DescriptorSet* descriptor_set, const uint32_t binding, const VkDescriptorType descriptor_type, const VkDescriptorImageInfo* descriptor_image_info)
    {			
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptor_set->GetVkDescriptorSet();
        descriptorWrite.dstBinding = binding;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = descriptor_type;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = descriptor_image_info;
        return descriptorWrite;
    }

    DescriptorSet::DescriptorSet(const DescriptorPool& descriptorPool, const std::vector<VkDescriptorSetLayout>& layouts, const Device* device) : device{device ? *device : GetDevice()}
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool.GetVkDescriptorPool();			
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layouts.data();
        VK_TRY(vkAllocateDescriptorSets(this->device.GetVkDevice(), &allocInfo, &vkDescriptorSet));
    }

    DescriptorSet::~DescriptorSet()
    {
    }

    void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet>& writeDescriptorSets)
    {
        vkUpdateDescriptorSets(device.GetVkDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
    }

    const VkDescriptorSet& DescriptorSet::GetVkDescriptorSet() const
    {
        return vkDescriptorSet;
    }
}