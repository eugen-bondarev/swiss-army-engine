#ifndef __Common_Math_h__
#define __Common_Math_h__

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <mat4x4.hpp>
#include <vec2.hpp>
#include <vec3.hpp>

using Vec2ui = glm::uvec2;
using Vec2i = glm::ivec2;
using Vec2f = glm::vec2;

using Vec3ui = glm::uvec3;
using Vec3i = glm::ivec3;
using Vec3f = glm::vec3;

namespace Math
{
    template <typename To, typename From>
    inline static constexpr To CastTo(const glm::vec<2, From, glm::defaultp> vec)
    {
        return { 
            static_cast<To::value_type>(vec.x), 
            static_cast<To::value_type>(vec.y) 
        };
    }

    template <typename To, typename From>
    inline static constexpr To CastTo(const glm::vec<3, From, glm::defaultp> vec)
    {
        return { 
            static_cast<To::value_type>(vec.x), 
            static_cast<To::value_type>(vec.y), 
            static_cast<T::value_type>(vec.z) 
        };
    }
}

using Mat4x4 = glm::mat4x4;

#endif