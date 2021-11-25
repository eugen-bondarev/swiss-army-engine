#ifndef __VK_Memory_SceneUniformBuffer_h__
#define __VK_Memory_SceneUniformBuffer_h__

#pragma once

#include "Buffer.h"

namespace VK
{    
    template <typename T>
    class SceneUniformBuffer : public Buffer
    {
    public:
        SceneUniformBuffer() : Buffer(sizeof(T), 1, &data, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        {
        }

        T& operator()()
        {
            return data;
        }

        const T& operator()() const
        {
            return data;
        }

        void Overwrite()
        {
            Update(&data);
        }

    private:
        T data;
    };
}

#endif