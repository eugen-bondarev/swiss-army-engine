#ifndef __Window_Window_h__
#define __Window_Window_h__

#pragma once

#include "GLFWHeader.h"

#include "../API/SwapChainBase.h"
#include "../Common/Common.h"
#include "Events/Callback.h"
#include "Time.h"

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
    RawWindow(const WindowMode mode = WindowMode::Windowed, const bool vSync = true, const Vec2ui size = {0u, 0u}, const std::string& title = "Window");
   ~RawWindow();

    void BeginFrame();
    void BeginFrameSubscribe(const Callback::BeginFrame& callback);

    void EndFrame();
    void EndFrameSubscribe(const Callback::EndFrame& callback);

    void ResizeSubscribe(const Callback::Resize& callback);
    void ResizeClear();

    void SetVSync(const bool value);
    bool GetVSync() const;

    void Destroy();

    bool IsRunning() const;
    GLFWwindow* GetHandle();
    Vec2ui GetSize() const;
    float GetAspectRatio() const;
    
    inline float GetDeltaTime() const
    {
        return time.GetDeltaTime();
    }

private:
    Vec2ui size;
    bool vSync;
    bool running{true};
    GLFWwindow* handle;
    Time time;
    Callback::Queue<Callback::Resize> resizeCallbacks;
    Callback::Queue<Callback::BeginFrame> beginFrameCallbacks;
    Callback::Queue<Callback::EndFrame> endFrameCallbacks;
    
    Base::SwapChain* swapChain;
    void SetSwapChain(Base::SwapChain* swapChain);

    RawWindow(const RawWindow&) = delete;
    RawWindow& operator=(const RawWindow&) = delete;
};

#endif