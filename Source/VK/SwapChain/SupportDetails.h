#ifndef __VK_SwapChain_SupportDetails_h__
#define __VK_SwapChain_SupportDetails_h__

#pragma once

#include "../Common.h"

namespace VK
{
    struct SupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
}

#endif