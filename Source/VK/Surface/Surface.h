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