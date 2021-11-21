#ifndef __VK_Entities_Commands_CommandBuffer_h__
#define __VK_Entities_Commands_CommandBuffer_h__

#pragma once

#include "../Device/Device.h"
#include "../../Common.h"
#include "CommandPool.h"

namespace VK
{
    class CommandBuffer
    {
    public:
        CommandBuffer(const CommandPool& commandPool, const Device* device = nullptr);
       ~CommandBuffer();

        const VkCommandBuffer& GetVkCommandBuffer() const;

    private:
        const Device& device;
        const CommandPool& commandPool;

        VkCommandBuffer vkCommandBuffer;
    };
}

#endif