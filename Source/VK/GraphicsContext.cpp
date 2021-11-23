#include "GraphicsContext.h"

#include "instance/instance.h"
#include "surface/surface.h"
#include "device/device.h"
#include "swap_chain/swap_chain.h"

namespace VK
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        instance = CreatePtr<Instance>();
        surface = CreatePtr<Surface>(*instance, window.GetHandle());
        device = CreatePtr<Device>(*instance);
        swapChain = CreatePtr<SwapChain>(window.GetHandle(), device.get());
    }

    GraphicsContext::~GraphicsContext()
    {
        
    }

    API::Type GraphicsContext::GetAPIType() const
    {
        return API::Type::Vulkan;
    }

    const Instance& GraphicsContext::GetInstance() const
    {
        return *instance;
    }

    const Surface& GraphicsContext::GetSurface() const
    {
        return *surface;
    }
    
    const Device& GraphicsContext::GetDevice() const
    {
        return *device;
    }

    SwapChain& GraphicsContext::GetSwapChain()
    {
        return *swapChain;
    }

    const Device& GetDevice()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->device;
    }

    const Surface& GetSurface()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->surface;
    }

    SwapChain& GetSwapChain()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->swapChain;
    }
}
