#ifndef __DX_WINDOW_H__
#define __DX_WINDOW_H__

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Graphics/SwapChainBase.h"
#include "../Common.h"
#include "Events/Callback.h"

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
friend class CallbackManager;

public:
    Window(const unsigned int Width = 1920u, const unsigned int Height = 1080u, const WindowMode Mode = WindowMode::Windowed, const std::string& Title = "Window");
   ~Window();

    bool IsRunning() const;

    Base::SwapChain* GetSwapChain();
    GLFWwindow* GetHandle();

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    void ResizeSubscribe(const Callback::Resize& Cb);
    void ResizeClear();

private:
    unsigned int Width, Height;
    Base::SwapChain* SwapChain;
    GLFWwindow* Handle;
    
    void SetSize(const unsigned int NewWidth, const unsigned int NewHeight);

    Callback::Queue<Callback::Resize> ResizeCallbacks;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

#endif