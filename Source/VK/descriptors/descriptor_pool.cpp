#include "descriptor_pool.h"

#include "../device/device.h"

#include "../GraphicsContext.h"

namespace VK
{
    DescriptorPool::DescriptorPool(const std::vector<VkDescriptorPoolSize>& poolSizes, const Device* device) : device{device ? *device : GetDevice()}
    {
        VkDescriptorPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        createInfo.maxSets = 1000 * poolSizes.size();
        createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        createInfo.pPoolSizes = poolSizes.data();
        VK_TRY(vkCreateDescriptorPool(this->device.GetVkDevice(), &createInfo, nullptr, &vkDescriptorPool));
    }

    DescriptorPool::~DescriptorPool()
    {
        vkDestroyDescriptorPool(device.GetVkDevice(), vkDescriptorPool, nullptr);
    }

    const VkDescriptorPool& DescriptorPool::GetVkDescriptorPool() const
    {
        return vkDescriptorPool;
    }
}