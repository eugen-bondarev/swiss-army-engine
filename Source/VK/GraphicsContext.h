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

    const Surface& GetSurface();
    const Device& GetDevice();
    SwapChain& GetSwapChain();
    const DescriptorPool& GetDefaultDescriptorPool();

    class GraphicsContext final : public API::GraphicsContext
    {
    friend const Surface& GetSurface();
    friend const Device& GetDevice();
    friend       SwapChain& GetSwapChain();
    friend const DescriptorPool& GetDefaultDescriptorPool();

    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext();

        API::Type GetAPIType() const override;

        const Instance& GetInstance() const;
        const Surface& GetSurface() const;
        const Device& GetDevice() const;
        SwapChain& GetSwapChain();
        const DescriptorPool& GetDefaultDescriptorPool() const;

    private:
        Ptr<Instance> instance;
        Ptr<Surface> surface;
        Ptr<Device> device;
        Ptr<SwapChain> swapChain;
        Ptr<DescriptorPool> defaultDescriptorPool;

        GraphicsContext(const GraphicsContext&) = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;
    };
}

#endif