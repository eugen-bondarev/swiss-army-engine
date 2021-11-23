#pragma once

#include "../Common.h"
#include "../Objects.h"

namespace VK
{
    class Sampler
    {
    public:
        Sampler(VkFilter filter, const Device* device = nullptr);
       ~Sampler();

        const VkSampler& GetVkSampler() const;

    private:
        const Device& device;
        VkSampler vkSampler;

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };
}