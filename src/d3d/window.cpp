#include "window.h"

Window::Window()
{
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    handle = glfwCreateWindow(800, 600, "Hello, world!", nullptr, nullptr);
}

Window::~Window()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}