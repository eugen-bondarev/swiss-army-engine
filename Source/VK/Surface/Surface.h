#ifndef __VK_Surface_Surface_h__
#define __VK_Surface_Surface_h__

#pragma once

#include "../Common.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    FORWARD_DECLARE(Instance);

    class Surface
    {
    public:
        Surface(RawWindow& window, const Instance& instnace);
       ~Surface();

        const VkSurfaceKHR& GetVkSurface() const;

    private:
        const Instance& instance;
        VkSurfaceKHR vkSurface;
    };
}

#endif