#ifndef __VK_Entities_SwapChain_SupportDetails_h__
#define __VK_Entities_SwapChain_SupportDetails_h__

#pragma once

#include "../../Common.h"
#include "../Surface/Surface.h"

namespace VK
{    
    struct SupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SupportDetails QuerySwapChainSupport(const Surface& surface, const VkPhysicalDevice& physicalDevice);
}

#endif