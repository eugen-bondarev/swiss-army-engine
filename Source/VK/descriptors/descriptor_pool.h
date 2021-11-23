#pragma once

#include "../Common.h"
#include "../Objects.h"

namespace VK
{
    class DescriptorPool
    {
    public:
        DescriptorPool(const std::vector<VkDescriptorPoolSize>& poolSizes, const Device* device = nullptr);
       ~DescriptorPool();

        const VkDescriptorPool& GetVkDescriptorPool() const;

    private:
        const Device& device;
        VkDescriptorPool vkDescriptorPool;

        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;
    };
}