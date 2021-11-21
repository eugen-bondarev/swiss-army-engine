#pragma once

#include "../Common.h"

namespace Engine
{
	namespace Vk
	{
		namespace Global
		{
			class Surface
			{
			public:
				Surface(GLFWwindow* handle);
				~Surface();

				VkSurfaceKHR GetVkSurface() const;

			private:
				VkSurfaceKHR vkSurface;
			};

			extern Surface *surface;
		}
	}
}