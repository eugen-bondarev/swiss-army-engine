#include "window.h"
#include "../dx/instance.h"

static size_t NumWindows{0};

Window::Window()
{
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    handle = glfwCreateWindow(800, 600, "Hello, world!", nullptr, nullptr);

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