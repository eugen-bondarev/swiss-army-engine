#include "GraphicsContext.h"

namespace VK
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        instance = CreatePtr<Instance>();
        surface = CreatePtr<Surface>(*instance, window);
        device = CreatePtr<Device>(*instance, *surface);
    }

    API::Type GraphicsContext::GetAPIType() const
    {
        return API::Type::Vulkan;
    }
}