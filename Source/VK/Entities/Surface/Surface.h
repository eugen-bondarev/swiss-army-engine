#ifndef __VK_Entities_Surface_Surface_h__
#define __VK_Entities_Surface_Surface_h__

#include "../../Common.h"
#include "../Instance/Instance.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    class Surface
    {
    public:
        Surface(const Instance& instance, RawWindow& window);
       ~Surface();

        const VkSurfaceKHR& GetVkSurface() const;

    private:
        const Instance& instance;
        VkSurfaceKHR vkSurface;

        Surface(const Surface&) = delete;
        Surface& operator=(const Surface&) = delete;
    };
}

#endif