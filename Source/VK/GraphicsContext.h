#ifndef __VK_GraphicsContext_h__
#define __VK_GraphicsContext_h__

#pragma once

#include "../API/GraphicsContext.h"

#include "Common.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    namespace Global
    {
        FORWARD_DECLARE(Instance);
        FORWARD_DECLARE(Surface);
        FORWARD_DECLARE(Device);
    }

    const Global::Surface& GetSurface();
    const Global::Device& GetDevice();

    class GraphicsContext final : public API::GraphicsContext
    {
    friend const Global::Surface& GetSurface();
    friend const Global::Device& GetDevice();

    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext();

        API::Type GetAPIType() const override;

        const Global::Instance& GetInstance() const;
        const Global::Surface& GetSurface() const;
        const Global::Device& GetDevice() const;

    private:
        Ptr<Global::Instance> instance;
        Ptr<Global::Surface> surface;
        Ptr<Global::Device> device;

        GraphicsContext(const GraphicsContext&) = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;
    };
}

#endif