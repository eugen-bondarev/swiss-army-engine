#include "support_details.h"

#include "../GraphicsContext.h"

namespace VK
{		
    SupportDetails QuerySwapChainSupport(VkPhysicalDevice device)
    {
        SupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, GetSurface().GetVkSurface(), &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, GetSurface().GetVkSurface(), &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, GetSurface().GetVkSurface(), &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, GetSurface().GetVkSurface(), &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, GetSurface().GetVkSurface(), &presentModeCount, details.presentModes.data());
        }

        return details;
    }
}