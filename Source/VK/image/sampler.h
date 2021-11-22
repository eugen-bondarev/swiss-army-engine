#pragma once

#include "../Common.h"

namespace VK
{
    class Sampler
    {
    public:
        Sampler(VkFilter filter);
        ~Sampler();

        VkSampler& GetVkSampler();

    private:
        VkSampler vkSampler;

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };

    namespace Global
    {
        extern Sampler* constantInterpolationSampler;
        extern Sampler* linearInterpolationSampler;
    }
}