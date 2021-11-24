#include "Surface.h"

#include "../../Window/RawWindow.h"
#include "../Instance/Instance.h"

namespace VK
{
    Surface::Surface(RawWindow& window, const Instance& instance) : instance{instance}
    {
        VK_TRY(glfwCreateWindowSurface(instance.GetVkInstance(), window.GetHandle(), nullptr, &vkSurface));
    }

    Surface::~Surface()
    {
        vkDestroySurfaceKHR(instance.GetVkInstance(), vkSurface, nullptr);
    }

    const VkSurfaceKHR& Surface::GetVkSurface() const
    {
        return vkSurface;
    }
}