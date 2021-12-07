#include "Mouse.h"

Mouse::Mouse(GLFWwindow* handle) : IInputDevice(handle)
{
}

void Mouse::ButtonCallback(int button, int action, int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
        {
            map[button] = ButtonState::Pressed;
            break;
        }
        case GLFW_RELEASE:
        {
            map[button] = ButtonState::Released;
            break;
        }
    }
}

bool Mouse::ButtonPressed(const Button button) const
{
    return Pressed(button);
}

bool Mouse::ButtonReleased(const Button button) const
{
    return Released(button);
}

bool Mouse::ButtonDown(const Button button) const
{
    return glfwGetMouseButton(handle, button);
}