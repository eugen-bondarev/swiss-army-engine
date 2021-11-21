#include "surface.h"

// #include "../../../window/window.h"
#include "../instance/instance.h"

namespace Engine
{
	namespace Vk
	{
		namespace Global
		{
			Surface* surface;

			Surface::Surface(GLFWwindow* handle)
			{
				VK_TRY(glfwCreateWindowSurface(instance->GetVkInstance(), handle, nullptr, &vkSurface));

				
			}

			Surface::~Surface()
			{
				vkDestroySurfaceKHR(instance->GetVkInstance(), vkSurface, nullptr);

				
			}

			VkSurfaceKHR Surface::GetVkSurface() const
			{
				return vkSurface;
			}
		}
	}
}