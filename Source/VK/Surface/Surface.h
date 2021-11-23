#ifndef __VK_Surface_Surface_h__
#define __VK_Surface_Surface_h__

#pragma once

#include "../Common.h"

namespace VK
{
    FORWARD_DECLARE(Instance);

    class Surface
    {
    public:
        Surface(const Instance& instnace, GLFWwindow* handle);
       ~Surface();

        VkSurfaceKHR GetVkSurface() const;

    private:
        const Instance& instance;
        VkSurfaceKHR vkSurface;
    };
}

#endif