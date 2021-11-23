#pragma once

#include "../Common.h"

namespace VK
{
    class Instance
    {
    public:
        Instance();
        ~Instance();

        const VkInstance GetVkInstance() const;

    private:
        VkInstance vkInstance;

        Instance(const Instance &) = delete;
        Instance &operator=(const Instance &) = delete;
    };
}