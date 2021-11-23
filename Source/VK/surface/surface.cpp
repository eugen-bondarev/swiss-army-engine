#include "surface.h"

#include "../instance/instance.h"

namespace VK
{
    Surface::Surface(const Instance& instance, GLFWwindow* handle) : instance{instance}
    {
        VK_TRY(glfwCreateWindowSurface(instance.GetVkInstance(), handle, nullptr, &vkSurface));
    }

    Surface::~Surface()
    {
        vkDestroySurfaceKHR(instance.GetVkInstance(), vkSurface, nullptr);
    }

    VkSurfaceKHR Surface::GetVkSurface() const
    {
        return vkSurface;
    }
}