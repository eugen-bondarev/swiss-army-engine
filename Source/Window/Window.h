#ifndef __Window_Window_h__
#define __Window_Window_h__

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Graphics/SwapChainBase.h"
#include "../Common/Common.h"
#include "Events/Callback.h"

namespace DX
{
    FORWARD_DECLARE(Instance);
}

namespace Base
{
    FORWARD_DECLARE(SwapChain);
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
    Window(const WindowMode mode = WindowMode::Windowed, const bool vSync = true, const unsigned int width = 0u, const unsigned int height = 0u, const std::string& title = "Window");
   ~Window();

    void BeginFrame();
    void EndFrame();

    void ResizeSubscribe(const Callback::Resize& NewResizeCallback);
    void ResizeClear();

    void SetVSync(const bool value);
    bool GetVSync() const;

    bool IsRunning() const;
    GLFWwindow* GetHandle();
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

private:
    unsigned int width;
    unsigned int height;
    bool vSync;
    GLFWwindow* handle;
    Callback::Queue<Callback::Resize> resizeCallbacks;
    
    Base::SwapChain* swapChain;
    void SetSwapChain(Base::SwapChain* swapChain);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

#endif