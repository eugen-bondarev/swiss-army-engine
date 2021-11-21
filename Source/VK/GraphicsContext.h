#ifndef __VK_GraphicsContext_h__
#define __VK_GraphicsContext_h__

#pragma once

#include "../API/GraphicsContext.h"
#include "Entities/SwapChain/SwapChain.h"
#include "Entities/Instance/Instance.h"
#include "Entities/Surface/Surface.h"
#include "Entities/Device/Device.h"
#include "Common.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    class GraphicsContext : public API::GraphicsContext
    {
    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext() = default;

        API::Type GetAPIType() const override;

    private:
        Ptr<Instance> instance;
        Ptr<Surface> surface;
        Ptr<Device> device;
        Ptr<SwapChain> swapChain;
    };
}

#endif