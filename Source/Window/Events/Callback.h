#ifndef __DX_Callback_h__
#define __DX_Callback_h__

#pragma once

#include "../../Common/Common.h"
#include <functional>
#include <vector>

struct GLFWwindow;

namespace Callback
{
    using Resize = std::function<void(const Vec2ui)>;

    template <typename T>
    using Queue = std::vector<T>;
}

#endif