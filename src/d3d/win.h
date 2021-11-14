#ifndef __D3D_WIN_H__
#define __D3D_WIN_H__

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class Win
{
public:
    using Handle_t = GLFWwindow*;

    Win()
    {
        glfwInit();
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        handle = glfwCreateWindow(800, 600, "Hello, world!", nullptr, nullptr);
    }

   ~Win()
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    Handle_t handle;
private:

    Win(const Win&) = delete;
    Win& operator=(const Win&) = delete;
};

#endif