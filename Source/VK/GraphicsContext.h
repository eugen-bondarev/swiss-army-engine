#ifndef __VK_GraphicsContext_h__
#define __VK_GraphicsContext_h__

#pragma once

#include "../API/GraphicsContext.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

FORWARD_DECLARE(RawWindow);

namespace VK
{
    class GraphicsContext : public API::GraphicsContext
    {
    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext() = default;

        API::Type GetAPIType() const override;

    private:
        VkInstance vkInstance;
    };
}

#endif