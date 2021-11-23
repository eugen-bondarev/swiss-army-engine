#ifndef __VK_Descriptors_DescriptorPool_h__
#define __VK_Descriptors_DescriptorPool_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    
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

#endif