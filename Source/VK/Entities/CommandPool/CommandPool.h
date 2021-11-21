#ifndef __VK_Entities_CommandPool_CommandPool_h__
#define __VK_Entities_CommandPool_CommandPool_h__

#pragma once

#include "../../Common.h"
#include "../Device/Device.h"

namespace VK
{
    class CommandPool
    {
    public:
        CommandPool();
       ~CommandPool();

        void Reset();

        const VkCommandPool& GetVkCommandPool() const;

    private:
        const Device& device;

        VkCommandPool vkCommandPool;

        CommandPool(const CommandPool&) = delete;
        CommandPool& operator=(const CommandPool&) = delete;
    };
}

#endif