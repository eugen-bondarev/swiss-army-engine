#ifndef __DX_WINDOW_H__
#define __DX_WINDOW_H__

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../common.h"

enum class WindowMode
{
    Windowed,
    Fullscreen,
    Borderless
};

class Window
{
public:
    using Handle_t = GLFWwindow*;

    Window(const unsigned int Width = 1920u, const unsigned int Height = 1080u, const WindowMode Mode = WindowMode::Windowed, const std::string& Title = "Window");
   ~Window();

    bool IsRunning() const;
    void Present(const UINT SyncInterval = 1u, const UINT Flags = 0u);

    Handle_t handle;
private:

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

#endif