#pragma once

#include "../Common.h"
#include "../Objects.h"

namespace VK
{
    class DescriptorPool
    {
    public:
        DescriptorPool(const std::vector<VkDescriptorPoolSize>& pool_sizes, const Global::Device* device = nullptr);
        ~DescriptorPool();

        VkDescriptorPool& GetVkDescriptorPool();

    private:
        const Global::Device& device;
        VkDescriptorPool vkDescriptorPool;

        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;
    };

    namespace Global
    {
        extern DescriptorPool* descriptorPool;
    }
}