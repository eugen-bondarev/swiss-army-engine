#ifndef __DX_Callback_h__
#define __DX_Callback_h__

#pragma once

#include <functional>
#include <vector>

struct GLFWwindow;

namespace Callback {

using Resize = std::function<void(unsigned int Width, unsigned int Height)>;

template <typename T>
using Queue = std::vector<T>;

}

#endif