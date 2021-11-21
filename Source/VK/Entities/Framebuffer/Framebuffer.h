#ifndef __VK_Entities_Framebuffer_Framebuffer_h__
#define __VK_Entities_Framebuffer_Framebuffer_h__

#pragma once

#include "../../Common.h"

namespace VK
{
    class Framebuffer
    {
    public:
        Framebuffer();
       ~Framebuffer();

    private:
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;
    };
}

#endif