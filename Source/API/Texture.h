#ifndef __API_Texture_h__
#define __API_Texture_h__

#pragma once

#include "../Common/Common.h"

namespace API
{
    class Texture
    {
    public:
        static Ptr<Texture> Create(const Vec2ui size, const unsigned char* data);
        virtual void Bind() = 0;
    };
}

#endif