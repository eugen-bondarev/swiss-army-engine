#include "Window.h"
#include "../DX/Instance.h"

static size_t NumWindows{0};

static void SizeCallback(GLFWwindow* Handle, int Width, int Height)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(Handle));
    window->GetSwapChain()->Resize(Width, Height);

    for (size_t i = 0; i < window->ResizeCallbacks.size(); i++)
    {
        window->ResizeCallbacks[i](Width, Height);
    }
}

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

    glfwSetWindowUserPointer(Handle, this);
    glfwSetWindowSizeCallback(Handle, SizeCallback);

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

GLFWwindow* Window::GetHandle()
{
    return Handle;
}

Base::SwapChain* Window::GetSwapChain()
{
    return SwapChain;
}