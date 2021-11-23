#ifndef __VK_Commands_CommandPool_h__
#define __VK_Commands_CommandPool_h__

#pragma once

#include "../Objects.h"
#include "../Common.h"

namespace VK
{
    class CommandPool
    {
    public:
        CommandPool(const Global::Device* device = nullptr);
        ~CommandPool();

        void Reset() const;

        VkCommandPool GetVkCommandPool() const;

    private:
        const Global::Device& device;

        VkCommandPool vkCommandPool;

        // CommandPool(const CommandPool &) = delete;
        // CommandPool &operator=(const CommandPool &) = delete;
    };

    namespace Global
    {
        extern CommandPool *commandPool;
    } // namespace Global
} // namespace VK

#endif