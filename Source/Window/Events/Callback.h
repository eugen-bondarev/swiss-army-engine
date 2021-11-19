#ifndef __DX_Callback_h__
#define __DX_Callback_h__

#pragma once

#include <functional>
#include <vector>

struct GLFWwindow;

namespace Callback
{
    using Resize = std::function<void(unsigned int width, unsigned int height)>;

    template <typename T>
    using Queue = std::vector<T>;
}

#endif