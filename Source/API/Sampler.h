#ifndef __API_Sampler_h__
#define __API_Sampler_h__

#pragma once

#include "../Common/Common.h"

namespace API
{
    class Sampler
    {
    public:
        static Ptr<Sampler> Create();
        virtual void Bind() = 0;
    };
}

#endif