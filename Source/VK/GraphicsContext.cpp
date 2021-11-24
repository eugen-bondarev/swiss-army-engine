#include "GraphicsContext.h"

#include "Descriptors/descriptorPool.h"
#include "SwapChain/SwapChain.h"
#include "Commands/CommandPool.h"
#include "Instance/Instance.h"
#include "Surface/Surface.h"
#include "Device/Device.h"
#include "Image/Sampler.h"

namespace VK
{
    GraphicsContext::GraphicsContext(RawWindow& window) : API::GraphicsContext(window)
    {
        instance = CreatePtr<Instance>(GetID());
        surface = CreatePtr<Surface>(window, *instance);
        device = CreatePtr<Device>(*instance);

        defaultConstInterpolationSampler = CreatePtr<Sampler>(VK_FILTER_NEAREST);
        defaultLinearInterpolationSampler = CreatePtr<Sampler>(VK_FILTER_LINEAR);
        
        swapChain = CreatePtr<SwapChain>(window, *device);
        defaultCommandPool = CreatePtr<CommandPool>(*device);
        
        defaultDescriptorPool = CreatePtr<DescriptorPool>(std::vector<VkDescriptorPoolSize> {				
            { VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }
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

    const CommandPool& GraphicsContext::GetDefaultCommandPool() const
    {
        return *defaultCommandPool;
    }

    const Sampler& GraphicsContext::GetDefaultConstInterpolationSampler() const
    {
        return *defaultConstInterpolationSampler;
    }

    const Sampler& GraphicsContext::GetDefaultLinearInterpolationSampler() const
    {
        return *defaultLinearInterpolationSampler;
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

    const CommandPool& GetDefaultCommandPool()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->defaultCommandPool;
    }

    const Sampler& GetDefaultConstInterpolationSampler()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->defaultConstInterpolationSampler;
    }

    const Sampler& GetDefaultLinearInterpolationSampler()
    {
        return *dynamic_cast<::VK::GraphicsContext*>(API::GetCurrentGraphicsContext())->defaultLinearInterpolationSampler;
    }
}
