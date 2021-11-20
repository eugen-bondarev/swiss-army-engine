#ifndef __API_Window_h__
#define __API_Window_h__

#pragma once

#include "../Window/RawWindow.h"
#include "GraphicsContext.h"

namespace API
{
    class Window : public RawWindow
    {
    public:
        template <typename... Args>
        Window(const Type type, Args&&... args) : RawWindow(std::forward<Args&&>(args)...)
        {
            graphicsContext = GraphicsContext::Create(*this, type);
        }

       ~Window() = default;

        GraphicsContext* GetGraphicsContext();
    
    private:
        Ptr<GraphicsContext> graphicsContext;

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
    };
}

#endif