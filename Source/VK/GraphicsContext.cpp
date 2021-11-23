#include "GraphicsContext.h"

#include "instance/instance.h"
#include "surface/surface.h"
#include "device/device.h"

namespace VK
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        instance = CreatePtr<Global::Instance>();
        surface = CreatePtr<Global::Surface>(*instance, window.GetHandle());
        device = CreatePtr<Global::Device>(*instance);
    }

    GraphicsContext::~GraphicsContext()
    {
        
    }

    API::Type GraphicsContext::GetAPIType() const
    {
        return API::Type::Vulkan;
    }

    const Global::Instance& GraphicsContext::GetInstance() const
    {
        return *instance;
    }

    const Global::Surface& GraphicsContext::GetSurface() const
    {
        return *surface;
    }
    
    const Global::Device& GraphicsContext::GetDevice() const
    {
        return *device;
    }

    const Global::Device& GetDevice()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->device;
    }

    const Global::Surface& GetSurface()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->surface;
    }
}
