#ifndef __VK_GraphicsContext_h__
#define __VK_GraphicsContext_h__

#pragma once

#include "../API/GraphicsContext.h"
#include "Common.h"

#include "Entities/CommandPool/CommandPool.h"
#include "Entities/SwapChain/SwapChain.h"
#include "Entities/Instance/Instance.h"
#include "Entities/Surface/Surface.h"
#include "Entities/Device/Device.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    Device* GetDevice();

    class GraphicsContext : public API::GraphicsContext
    {
    friend Device* GetDevice();

    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext() = default;

        API::Type GetAPIType() const override;

    private:
        Ptr<Instance> instance;
        Ptr<Surface> surface;
        Ptr<Device> device;
        Ptr<SwapChain> swapChain;
        Ptr<CommandPool> commandPool;
    };
}

#endif