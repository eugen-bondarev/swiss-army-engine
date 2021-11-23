#include "vk.h"

#include "GraphicsContext.h"

namespace VK
{
    void Bootstrap(GLFWwindow* handle)
    {
        GraphicsContext* ctx = dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext());
    }

    void Shutdown()
    {
    }
}