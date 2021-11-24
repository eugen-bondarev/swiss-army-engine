#ifndef __VK_Instance_Instance_h__
#define __VK_Instance_Instance_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Debug);

    class Instance
    {
    public:
        Instance(const unsigned int id);
       ~Instance();

        const VkInstance GetVkInstance() const;

    private:
        Ptr<Debug> valid;
        VkInstance vkInstance;

        Instance(const Instance &) = delete;
        Instance &operator=(const Instance &) = delete;
    };
}

#endif