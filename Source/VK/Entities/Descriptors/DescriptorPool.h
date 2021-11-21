#ifndef __VK_Entities_Descriptors_DescriptorPool_h__
#define __VK_Entities_Descriptors_DescriptorPool_h__

#pragma once

#include "../Device/Device.h"
#include "../../Common.h"

namespace VK
{
    class DescriptorPool
    {
    public:
        DescriptorPool(const Device* device = nullptr);
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