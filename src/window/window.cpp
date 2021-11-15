#include "window.h"
#include "../dx/instance.h"

static size_t NumWindows{0};

Window::Window(const unsigned int Width, const unsigned int Height, const WindowMode Mode, const std::string& Title)
{
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    switch (Mode)
    {
        case WindowMode::Windowed:
        {
            handle = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
            glfwMaximizeWindow(handle);
            break;
        }

        case WindowMode::Fullscreen:
        {
            handle = glfwCreateWindow(Width, Height, Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            break;
        }

        case WindowMode::Borderless:
        {
            const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
            handle = glfwCreateWindow(Width, Height, Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            break;
        }
    }

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

void Window::Present(const UINT SyncInterval, const UINT Flags)
{            
    HRESULT hr = DX::GetSwapChain()->Present(SyncInterval, Flags);   

    if (FAILED(hr))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw EXCEPTION_WHAT(std::to_string(DX::GetDevice()->GetDeviceRemovedReason()));
        }
        else
        {
            throw EXCEPTION();
        }
    }
}