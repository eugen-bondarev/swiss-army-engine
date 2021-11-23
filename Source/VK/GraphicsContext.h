#ifndef __VK_GraphicsContext_h__
#define __VK_GraphicsContext_h__

#pragma once

#include "../API/GraphicsContext.h"

#include "Common.h"

FORWARD_DECLARE(RawWindow);

namespace VK
{
    class GraphicsContext final : public API::GraphicsContext
    {
    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext();

        API::Type GetAPIType() const override;

    private:
        GraphicsContext(const GraphicsContext&) = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;
    };
}

#endif