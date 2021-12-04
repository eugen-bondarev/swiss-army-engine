#ifndef __Logic_ISpace_h__
#define __Logic_ISpace_h__

#pragma once

namespace Logic
{
    class ISpace
    {
    public:
        virtual void UpdateProjectionMatrix() = 0;
    };
}

#endif