#pragma once


#include "../Common.h"

namespace VK
{
    class CommandPool
    {
    public:
        CommandPool();
        ~CommandPool();

        void Reset() const;

        VkCommandPool GetVkCommandPool() const;

    private:
        VkCommandPool vkCommandPool;

        CommandPool(const CommandPool &) = delete;
        CommandPool &operator=(const CommandPool &) = delete;
    };

    namespace Global
    {
        extern CommandPool *commandPool;
    }
}