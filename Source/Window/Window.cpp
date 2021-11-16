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