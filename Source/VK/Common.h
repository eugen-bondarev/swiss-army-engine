#ifndef __VK_Common_h__
#define __VK_Common_h__

#include "../Common/Common.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace VK
{
    
}

#ifndef NDEBUG
#   define VK_TRY(exp)\
        if (exp != VK_SUCCESS) throw std::runtime_error(#exp)
        // MY_ASSERT(exp == VK_SUCCESS)
#else
#   define VK_TRY(exp)\
        exp
#endif

#endif