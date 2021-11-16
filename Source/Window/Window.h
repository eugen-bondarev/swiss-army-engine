#ifndef __DX_WINDOW_H__
#define __DX_WINDOW_H__

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Graphics/SwapChainBase.h"
#include "../Common.h"

#include <functional>

enum class WindowMode
{
    Windowed,
    Fullscreen,
    Borderless
};

namespace DX {

class Instance;

}

class Window
{
friend class DX::Instance;

public:
    Window(const unsigned int Width = 1920u, const unsigned int Height = 1080u, const WindowMode Mode = WindowMode::Windowed, const std::string& Title = "Window");
   ~Window();

    bool IsRunning() const;

    GLFWwindow* GetHandle();
    Base::SwapChain* GetSwapChain();

    std::vector<std::function<void(int, int)>> ResizeCallbacks;

private:
    GLFWwindow* Handle;
    
    Base::SwapChain* SwapChain;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

#endif