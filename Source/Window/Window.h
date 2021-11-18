#ifndef __DX_WINDOW_H__
#define __DX_WINDOW_H__

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Graphics/SwapChainBase.h"
#include "../Common.h"
#include "Events/Callback.h"

namespace DX
{
    FORWARD_DECLARE(Instance);
}

enum class WindowMode
{
    Windowed,
    Fullscreen,
    Borderless
};

class Window
{
friend class DX::Instance;
friend class CallbackManager;

public:
    Window(const WindowMode mode = WindowMode::Windowed, const unsigned int width = 0u, const unsigned int height = 0u, const std::string &title = "Window");
   ~Window();

    void ResizeSubscribe(const Callback::Resize& NewResizeCallback);
    void ResizeClear();

    bool IsRunning() const;
    GLFWwindow* GetHandle();
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

private:
    unsigned int width;
    unsigned int height;
    GLFWwindow* handle;
    Callback::Queue<Callback::Resize> resizeCallbacks;

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
};

#endif