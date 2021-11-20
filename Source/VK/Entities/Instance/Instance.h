#ifndef __VK_Entities_Instance_Instance_h__
#define __VK_Entities_Instance_Instance_h__

#include "../../Common.h"

namespace VK
{
    class Instance
    {
    public:
        Instance();
       ~Instance();

        const VkInstance& GetVkInstance() const;

    private:
        VkInstance vkInstance;

        Instance(const Instance&) = delete;
        Instance& operator=(const Instance&) = delete;
    };
}

#endif