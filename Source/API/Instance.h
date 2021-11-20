#ifndef __API_Instance_h__
#define __API_Instance_h__

#pragma once

#include "../Common/Common.h"
#include "../Window/Window.h"

namespace API
{
    enum class Type
    {
        DirectX,
        Vulkan,
        OpenGL
    };

    class Instance
    {
    public:
        Instance(Window& window);
       ~Instance() = default;

        static Ptr<Instance> Create(Window& window, const Type type);
        virtual Type GetAPIType() const = 0;

    protected:
        const Window& window;
    };

    void MakeInstanceCurrent(Instance* newContext);
    Instance* GetInstance(Window* window);
    Instance* GetCurrentInstance();
}

#endif