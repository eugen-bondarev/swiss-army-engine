#include "GraphicsContext.h"

#include "instance/instance.h"
#include "surface/surface.h"
#include "device/device.h"
#include "swap_chain/swap_chain.h"
#include "descriptors/descriptor_pool.h"

namespace VK
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        instance = CreatePtr<Instance>();
        surface = CreatePtr<Surface>(*instance, window.GetHandle());
        device = CreatePtr<Device>(*instance);
        swapChain = CreatePtr<SwapChain>(window.GetHandle(), device.get());
        
        defaultDescriptorPool = CreatePtr<DescriptorPool>(std::vector<VkDescriptorPoolSize> {				
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        });
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

    const DescriptorPool& GraphicsContext::GetDefaultDescriptorPool() const
    {
        return *defaultDescriptorPool;
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

    const DescriptorPool& GetDefaultDescriptorPool()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->defaultDescriptorPool;
    }
}
