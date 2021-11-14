#ifndef __D3D_WINDOW_H__
#define __D3D_WINDOW_H__

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class Window
{
public:
    using Handle_t = GLFWwindow*;

    Window();
   ~Window();

    Handle_t handle;
private:

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

#endif