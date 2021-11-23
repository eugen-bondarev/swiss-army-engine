#ifndef __API_GraphicsContext_h__
#define __API_GraphicsContext_h__

#pragma once

#include "../Window/RawWindow.h"
#include "../Common/Common.h"

namespace API
{
    enum class Type
    {
        DirectX,
        Vulkan,
        OpenGL
    };

    class GraphicsContext
    {
    public:
        GraphicsContext(RawWindow& window);
       ~GraphicsContext() = default;

        static Ptr<GraphicsContext> Create(RawWindow& window, const Type type);
        virtual Type GetAPIType() const = 0;

        RawWindow& GetWindow();

    protected:
        RawWindow& window;
    };

    void MakeGraphicsContextCurrent(GraphicsContext* newContext);
    GraphicsContext* GetGraphicsContext(RawWindow* window);
    GraphicsContext* GetCurrentGraphicsContext();
}

#endif