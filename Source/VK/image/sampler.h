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

        VkSampler& GetVkSampler();

    private:
        const Device& device;
        VkSampler vkSampler;

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };

    extern Sampler* constantInterpolationSampler;
    extern Sampler* linearInterpolationSampler;
}