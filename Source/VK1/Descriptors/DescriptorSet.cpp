#include "DescriptorSet.h"

#include "../GraphicsContext.h"
// #include "../Image/ImageView.h"
// #include "../Image/Sampler.h"
#include "../Device/Device.h"
#include "DescriptorPool.h"

namespace VK
{
    VkWriteDescriptorSet CreateWriteDescriptorSet(DescriptorSet* descriptor_set, uint32_t binding, VkDescriptorType descriptor_type, const VkDescriptorBufferInfo* descriptor_buffer_info)
    {
        VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = descriptor_set->GetVkDescriptorSet();
        descriptor_write.dstBinding = binding;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType = descriptor_type;
        descriptor_write.descriptorCount = 1;
        descriptor_write.pBufferInfo = descriptor_buffer_info;

        return descriptor_write;
    }

    VkWriteDescriptorSet CreateWriteDescriptorSet(DescriptorSet* descriptor_set, uint32_t binding, VkDescriptorType descriptor_type, const VkDescriptorImageInfo* descriptor_image_info)
    {			
        VkWriteDescriptorSet descriptor_write = {};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = descriptor_set->GetVkDescriptorSet();
        descriptor_write.dstBinding = binding;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType = descriptor_type;
        descriptor_write.descriptorCount = 1;
        descriptor_write.pImageInfo = descriptor_image_info;

        return descriptor_write;
    }

    DescriptorSet::DescriptorSet(const DescriptorPool& descriptorPool, const std::vector<VkDescriptorSetLayout>& layouts, const Device* device) : device{device ? *device : GetDevice()}
    {
        VkDescriptorSetAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = descriptorPool.GetVkDescriptorPool();			
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = layouts.data();
        VK_TRY(vkAllocateDescriptorSets(this->device.GetVkDevice(), &alloc_info, &vkDescriptorSet));
    }

    DescriptorSet::~DescriptorSet()
    {
    }

    void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet>& write_descriptor_sets)
    {
        vkUpdateDescriptorSets(device.GetVkDevice(), static_cast<uint32_t>(write_descriptor_sets.size()), write_descriptor_sets.data(), 0, nullptr);
    }

    VkDescriptorSet& DescriptorSet::GetVkDescriptorSet()
    {
        return vkDescriptorSet;
    }
}