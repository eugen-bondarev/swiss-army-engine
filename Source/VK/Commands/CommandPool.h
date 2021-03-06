#ifndef __VK_Commands_CommandPool_h__
#define __VK_Commands_CommandPool_h__

#pragma once

#include "../GraphicsContext.h"
#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Device);
    
    class CommandPool
    {
    public:
        CommandPool(const Device& device = GetDevice());
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