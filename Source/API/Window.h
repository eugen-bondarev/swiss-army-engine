#ifndef __API_Window_h__
#define __API_Window_h__

#pragma once

#include "../Window/RawWindow.h"
#include "Instance.h"

namespace API
{
    class Window : public RawWindow
    {
    public:
        template <typename... Args>
        Window(const Type type, Args&&... args) : RawWindow(std::forward<Args&&>(args)...)
        {
            instance = Instance::Create(*this, type);
        }

       ~Window() = default;
    
    private:
        Ptr<Instance> instance;

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
    };
}

#endif