#ifndef __VK_Entities_Device_QueueFamily_h__
#define __VK_Entities_Device_QueueFamily_h__

#pragma once

#include "../../Common.h"
#include "../Surface/Surface.h"

#include <optional>

namespace VK
{
    namespace Queues
    {
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool IsComplete() const;
        };

        extern QueueFamilyIndices indices;
        extern VkQueue graphicsQueue;
        extern VkQueue presentQueue;

        QueueFamilyIndices FindQueueFamilies(const Surface& surface, VkPhysicalDevice physicalDevice);
    }
}

#endif