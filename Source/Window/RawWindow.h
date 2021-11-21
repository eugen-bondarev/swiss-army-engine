#ifndef __Window_Window_h__
#define __Window_Window_h__

#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Graphics/SwapChainBase.h"
#include "../Common/Common.h"
#include "Events/Callback.h"

namespace DX
{
    FORWARD_DECLARE(GraphicsContext);
}

namespace VK
{
    FORWARD_DECLARE(GraphicsContext);
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

class RawWindow
{
friend class DX::GraphicsContext;
friend class VK::GraphicsContext;
friend class CallbackManager;

public:
    RawWindow(const WindowMode mode = WindowMode::Windowed, const bool vSync = true, const unsigned int width = 0u, const unsigned int height = 0u, const std::string& title = "Window");
   ~RawWindow();

    void BeginFrame();
    void EndFrame();

    void ResizeSubscribe(const Callback::Resize& callback);
    void ResizeClear();

    void SetVSync(const bool value);
    bool GetVSync() const;

    void Destroy();

    bool IsRunning() const;
    GLFWwindow* GetHandle();
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

private:
    unsigned int width;
    unsigned int height;
    bool vSync;
    bool running{true};
    GLFWwindow* handle;
    Callback::Queue<Callback::Resize> resizeCallbacks;
    
    Base::SwapChain* swapChain;
    void SetSwapChain(Base::SwapChain* swapChain);

    RawWindow(const RawWindow&) = delete;
    RawWindow& operator=(const RawWindow&) = delete;
};

#endif