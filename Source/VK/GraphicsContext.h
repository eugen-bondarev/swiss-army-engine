#ifndef __VK_GraphicsContext_h__
#define __VK_GraphicsContext_h__

#pragma once

#include "../API/GraphicsContext.h"
#include "Common.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    FORWARD_DECLARE(Instance);
    FORWARD_DECLARE(Surface);
    FORWARD_DECLARE(Device);
    FORWARD_DECLARE(SwapChain);
    FORWARD_DECLARE(DescriptorPool);
    FORWARD_DECLARE(CommandPool);
    FORWARD_DECLARE(Sampler);

    const Instance&       GetInstance();
    const Surface&        GetSurface();
    const Device&         GetDevice();
          SwapChain&      GetSwapChain();
    const DescriptorPool& GetDefaultDescriptorPool();
    const CommandPool&    GetDefaultCommandPool();
    const Sampler&        GetDefaultConstInterpolationSampler();
    const Sampler&        GetDefaultLinearInterpolationSampler();

    class GraphicsContext final : public API::GraphicsContext
    {
    friend const Instance&       GetInstance();
    friend const Surface&        GetSurface();
    friend const Device&         GetDevice();
    friend       SwapChain&      GetSwapChain();
    friend const DescriptorPool& GetDefaultDescriptorPool();
    friend const CommandPool&    GetDefaultCommandPool();
    friend const Sampler&        GetDefaultConstInterpolationSampler();
    friend const Sampler&        GetDefaultLinearInterpolationSampler();

    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext();

        API::Type GetAPIType() const override;

        const Instance&       GetInstance() const;
        const Surface&        GetSurface() const;
        const Device&         GetDevice() const;
              SwapChain&      GetSwapChain();
        const DescriptorPool& GetDefaultDescriptorPool() const;
        const CommandPool&    GetDefaultCommandPool() const;
        const Sampler&        GetDefaultConstInterpolationSampler() const;
        const Sampler&        GetDefaultLinearInterpolationSampler() const;

    private:
        Ptr<Instance> instance;
        Ptr<Surface> surface;
        Ptr<Device> device;
        Ptr<SwapChain> swapChain;
        Ptr<DescriptorPool> defaultDescriptorPool;
        Ptr<CommandPool> defaultCommandPool;
        Ptr<Sampler> defaultConstInterpolationSampler;
        Ptr<Sampler> defaultLinearInterpolationSampler;

        GraphicsContext(const GraphicsContext&) = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;
    };
    
    GraphicsContext& GetCurrentGraphicsContext();
}

#endif