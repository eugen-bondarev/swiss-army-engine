#pragma once

#include "../Common.h"
#include "../instance/instance.h"

namespace VK
{
    namespace Global
    {
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

        // extern Surface *surface;
    }
}