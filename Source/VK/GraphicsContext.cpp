#include "GraphicsContext.h"

namespace VK
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        instance = CreatePtr<Instance>();
        surface = CreatePtr<Surface>(*instance, window);
        device = CreatePtr<Device>(*instance, *surface);
        swapChain = CreatePtr<SwapChain>(window, *surface, *device);
        commandPool = CreatePtr<CommandPool>();
        commandBuffer = CreatePtr<CommandBuffer>(*commandPool);
        
        window.SetSwapChain(swapChain.get());
    }

    API::Type GraphicsContext::GetAPIType() const
    {
        return API::Type::Vulkan;
    }

    Device* GetDevice()
    {
        return dynamic_cast<VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->device.get();
    }
}