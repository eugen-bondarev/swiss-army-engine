#ifndef __VK_Image_Sampler_h__
#define __VK_Image_Sampler_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    
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

#endif