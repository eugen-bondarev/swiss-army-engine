#include "Window.h"
#include "../DX/Instance.h"

static size_t NumWindows{0};

class CallbackManager
{
public:
    template <typename... Args>
    static void Issue(const Callback::Queue<std::function<void(Args...)>> &callbacks, Args... args)
    {
        for (const auto &callback : callbacks)
        {
            callback(std::forward<Args>(args)...);
        }
    }

    static void SizeCallback(GLFWwindow* handle, int width, int height)
    {
        Window* window = static_cast<Window* >(glfwGetWindowUserPointer(handle));
        window->width = width; window->height = height;
        Issue(window->resizeCallbacks, static_cast<unsigned int>(width), static_cast<unsigned int>(height));
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

Window::Window(const WindowMode mode, const unsigned int width, const unsigned int height, const std::string &title)
{
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    unsigned int finalWidth{width}, finalHeight{height};

    if (finalWidth == 0u || finalHeight == 0u)
    {
        GetMonitorResolution(finalWidth, finalHeight);
    }

    switch (mode)
    {
        case WindowMode::Windowed:
        {
            handle = glfwCreateWindow(finalWidth, finalHeight, title.c_str(), nullptr, nullptr);
            glfwMaximizeWindow(handle);
            break;
        }

        case WindowMode::Fullscreen:
        {
            handle = glfwCreateWindow(finalWidth, finalHeight, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            break;
        }

        case WindowMode::Borderless:
        {
            const GLFWvidmode* videoMode{glfwGetVideoMode(glfwGetPrimaryMonitor())};
            glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
            handle = glfwCreateWindow(finalWidth, finalHeight, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            break;
        }
    }

    int _width, _height;
    glfwGetFramebufferSize(handle, &_width, &_height);
    this->width = static_cast<unsigned int>(_width);
    this->height = static_cast<unsigned int>(_height);

    glfwSetWindowUserPointer(handle, this);
    glfwSetWindowSizeCallback(handle, CallbackManager::SizeCallback);

    NumWindows++;
}

Window::~Window()
{
    NumWindows--;

    glfwDestroyWindow(handle);

    if (NumWindows == 0)
    {
        glfwTerminate();
    }
}

bool Window::IsRunning() const
{
    return !glfwWindowShouldClose(handle);
}

unsigned int Window::GetWidth() const
{
    return width;
}

unsigned int Window::GetHeight() const
{
    return height;
}

GLFWwindow* Window::GetHandle()
{
    return handle;
}

void Window::ResizeSubscribe(const Callback::Resize& callback)
{
    resizeCallbacks.push_back(callback);
}

void Window::ResizeClear()
{
    resizeCallbacks.clear();
}