#include "Window.h"
#include "../DX/Instance.h"

static size_t NumWindows{0};

class CallbackManager
{
public:
    template <typename... Arguments>
    static void Issue(const Callback::Queue<std::function<void(Arguments...)>>& Callbacks, Arguments... Args)
    {
        for (const auto& callback : Callbacks)
        {
            callback(std::forward<Arguments>(Args)...);
        }
    }

    static void SizeCallback(GLFWwindow* Handle, int Width, int Height)
    {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(Handle));
        window->SetSize(Width, Height);
        Issue(window->ResizeCallbacks, static_cast<unsigned int>(Width), static_cast<unsigned int>(Height));
    }

private:
    CallbackManager() = delete;
    CallbackManager(const CallbackManager&) = delete;
    CallbackManager& operator=(const CallbackManager&) = delete;
};

Window::Window(const unsigned int Width, const unsigned int Height, const WindowMode Mode, const std::string& Title)
{
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    switch (Mode)
    {
        case WindowMode::Windowed:
        {
            Handle = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
            glfwMaximizeWindow(Handle);
            break;
        }

        case WindowMode::Fullscreen:
        {
            Handle = glfwCreateWindow(Width, Height, Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            break;
        }

        case WindowMode::Borderless:
        {
            const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
            Handle = glfwCreateWindow(Width, Height, Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            break;
        }
    }

    int width, height;
    glfwGetFramebufferSize(Handle, &width, &height);
    this->Width = static_cast<unsigned int>(width); 
    this->Height = static_cast<unsigned int>(height);

    glfwSetWindowUserPointer(Handle, this);
    glfwSetWindowSizeCallback(Handle, CallbackManager::SizeCallback);

    NumWindows++;
}

Window::~Window()
{
    NumWindows--;

    glfwDestroyWindow(Handle);

    if (NumWindows == 0)
    {
        glfwTerminate();
    }
}

bool Window::IsRunning() const
{
    return !glfwWindowShouldClose(Handle);
}

unsigned int Window::GetWidth() const
{
    return Width;
}

unsigned int Window::GetHeight() const
{
    return Height;
}

void Window::SetSize(const unsigned int NewWidth, const unsigned int NewHeight)
{
    Width = NewWidth; Height = NewHeight;
    SwapChain->SetSize(Width, Height);
}

GLFWwindow* Window::GetHandle()
{
    return Handle;
}

Base::SwapChain* Window::GetSwapChain()
{
    return SwapChain;
}

void Window::ResizeSubscribe(const Callback::Resize& Cb)
{
    ResizeCallbacks.push_back(Cb);
}

void Window::ResizeClear()
{
    ResizeCallbacks.clear();
}