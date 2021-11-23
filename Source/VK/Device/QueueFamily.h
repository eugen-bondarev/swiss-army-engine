#ifndef __VK_Device_QueueFamily_h__
#define __VK_Device_QueueFamily_h__

#pragma once

#include "../Common.h"
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
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);

        extern VkQueue graphicsQueue;
        extern VkQueue presentQueue;
    }
}

#endif