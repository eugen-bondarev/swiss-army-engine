#include "Surface.h"

#include "../../../API/Window.h"

namespace VK
{
    Surface::Surface(const Instance& instance, RawWindow& window) : instance{instance}
    {
        glfwCreateWindowSurface(instance.GetVkInstance(), window.GetHandle(), nullptr, &vkSurface);
    }

    Surface::~Surface()
    {
        vkDestroySurfaceKHR(instance.GetVkInstance(), vkSurface, nullptr);
    }
}