#ifndef __Logic_IOrthogonal_h__
#define __Logic_IOrthogonal_h__

#pragma once

#include "ISpace.h"

namespace Logic
{
    class IOrthogonal : public ISpace
    {
    public:
        void SetLeft(const float value) { left = value; UpdateProjectionMatrix(); }
        void SetRight(const float value) { right = value; UpdateProjectionMatrix(); }
        void SetBottom(const float value) { bottom = value; UpdateProjectionMatrix(); }
        void SetTop(const float value) { top = value; UpdateProjectionMatrix(); }
        void Set(const float left, const float right, const float bottom, const float top) { this->left = left; this->right = right; this->bottom = bottom; this->top = top; UpdateProjectionMatrix(); }

        float GetLeft() const { return left; }
        float GetRight() const { return right; }
        float GetBottom() const { return bottom; }
        float GetTop() const { return top; }

    protected:
        float left;
        float right;
        float bottom;
        float top;
    };
}

#endif