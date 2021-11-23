#pragma once

#include "../Common.h"
#include "../Objects.h"

namespace VK
{
    class Sampler
    {
    public:
        Sampler(VkFilter filter, const Global::Device* device = nullptr);
        ~Sampler();

        VkSampler& GetVkSampler();

    private:
        const Global::Device& device;
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