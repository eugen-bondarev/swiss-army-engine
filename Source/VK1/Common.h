#ifndef __VK_Common_h__
#define __VK_Common_h__

#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Common/Common.h"

namespace VK
{
    struct Vec2
    {
        float x{0}, y{0};
    };
}

#define VK_TRY(exp)\
    if (exp != VK_SUCCESS) throw std::runtime_error(#exp)

#define VK_ASSERT(exp)\
    assert(exp)

#endif