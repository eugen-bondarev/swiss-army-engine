#include "window.h"
#include "d3d_instance.h"

static size_t numWindows{0};

Window::Window()
{
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    handle = glfwCreateWindow(800, 600, "Hello, world!", nullptr, nullptr);

    numWindows++;
}

Window::~Window()
{
    numWindows--;

    glfwDestroyWindow(handle);

    if (numWindows == 0)
    {
        glfwTerminate();
    }
}

bool Window::IsRunning() const
{
    return !glfwWindowShouldClose(handle);
}

void Window::Present(const UINT syncInterval, const UINT flags)
{            
    HRESULT hr = Swapchain()->Present(syncInterval, flags);   

    if (FAILED(hr))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw EXCEPTION_WHAT(std::to_string(Device()->GetDeviceRemovedReason()));
        }
        else
        {
            throw EXCEPTION();
        }
    }
}