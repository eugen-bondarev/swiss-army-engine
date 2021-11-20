#ifndef __API_Instance_h__
#define __API_Instance_h__

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

    class Instance
    {
    public:
        Instance(RawWindow& window);
       ~Instance() = default;

        static Ptr<Instance> Create(RawWindow& window, const Type type);
        virtual Type GetAPIType() const = 0;

    protected:
        const RawWindow& window;
    };

    void MakeInstanceCurrent(Instance* newContext);
    Instance* GetInstance(RawWindow* window);
    Instance* GetCurrentInstance();
}

#endif