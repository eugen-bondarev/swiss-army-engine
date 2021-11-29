#include "RawWindow.h"

#include <mutex>

static size_t numWindows{0};
static bool glfwInitialized{false};

class CallbackManager
{
public:
    template <typename... Args>
    static void Issue(const Callback::Queue<std::function<void(Args...)>> &callbacks, Args... args)
    {
        for (const auto& callback : callbacks)
        {
            callback(std::forward<Args>(args)...);
        }
    }

    static void SizeCallback(GLFWwindow* handle, int width, int height)
    {
        RawWindow* window = static_cast<RawWindow*>(glfwGetWindowUserPointer(handle));
        window->size = {width, height};
        Issue(window->resizeCallbacks, window->size);
    }

private:
    CallbackManager() = delete;
    CallbackManager(const CallbackManager&) = delete;
    CallbackManager& operator=(const CallbackManager&) = delete;
};

static void GetMonitorResolution(unsigned int& width, unsigned int& height)
{    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    width = mode->width;
    height = mode->height;
}

RawWindow::RawWindow(const WindowMode mode, const bool vSync, const Vec2ui size, const std::string& title) : vSync{vSync}
{
    {
        static std::mutex glfwInitMutex;
        std::lock_guard glfwInitLockGuard(glfwInitMutex);
        if (!glfwInitialized)
        {
            glfwInit();
            glfwInitialized = true;
        }
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    Vec2ui finalSize{size};
    if (finalSize.x == 0u || finalSize.y == 0u)
    {
        GetMonitorResolution(finalSize.x, finalSize.y);
    }

    switch (mode)
    {
        case WindowMode::Windowed:
        {
            handle = glfwCreateWindow(finalSize.x, finalSize.y, title.c_str(), nullptr, nullptr);

            if (size.x == 0u || size.y == 0u)
            {
                glfwMaximizeWindow(handle);
            }
            break;
        }

        case WindowMode::Fullscreen:
        {
            handle = glfwCreateWindow(finalSize.x, finalSize.y, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            break;
        }

        case WindowMode::Borderless:
        {
            const GLFWvidmode* videoMode{glfwGetVideoMode(glfwGetPrimaryMonitor())};
            glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
            handle = glfwCreateWindow(finalSize.x, finalSize.y, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            break;
        }
    }

    int _width, _height;
    glfwGetFramebufferSize(handle, &_width, &_height);
    this->size = {static_cast<unsigned int>(_width), static_cast<unsigned int>(_height)};

    glfwSetWindowUserPointer(handle, this);
    glfwSetWindowSizeCallback(handle, CallbackManager::SizeCallback);

    // For debugging.
    glfwSetWindowPos(handle, 50, 50);

    static std::mutex mutex;
    std::lock_guard lock(mutex);
    numWindows++;
}

RawWindow::~RawWindow()
{
    numWindows--;

    glfwDestroyWindow(handle);

    if (numWindows == 0)
    {
        glfwTerminate();
        glfwInitialized = false;
    }
}

void RawWindow::BeginFrame()
{
    glfwPollEvents();
    time.BeginFrame();
}

void RawWindow::EndFrame()
{
    MY_ASSERT(swapChain != nullptr);
    swapChain->Present(static_cast<unsigned int>(vSync), 0u);
    time.EndFrame();
}

bool RawWindow::IsRunning() const
{
    return !glfwWindowShouldClose(handle) && running;
}

void RawWindow::Destroy()
{
    running = false;
}

void RawWindow::SetVSync(const bool value)
{
    vSync = value;
}

bool RawWindow::GetVSync() const
{
    return vSync;
}

Vec2ui RawWindow::GetSize() const
{
    return size;
}

float RawWindow::GetAspectRatio() const
{
    return static_cast<float>(size.x) / static_cast<float>(size.y);
}

void RawWindow::SetSwapChain(Base::SwapChain* swapChain)
{
    this->swapChain = swapChain;
}

GLFWwindow* RawWindow::GetHandle()
{
    return handle;
}

void RawWindow::ResizeSubscribe(const Callback::Resize& callback)
{
    resizeCallbacks.push_back(callback);
}

void RawWindow::ResizeClear()
{
    resizeCallbacks.clear();
}