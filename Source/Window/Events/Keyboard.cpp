#include "Keyboard.h"

Keyboard::Keyboard(GLFWwindow* handle) : IInputDevice(handle)
{
}

void Keyboard::KeyCallback(int key, int scancode, int action, int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
        {
            map[key] = KeyState::Pressed;
            break;
        }
        case GLFW_RELEASE:
        {
            map[key] = KeyState::Released;
            break;
        }
    }
}

bool Keyboard::KeyPressed(const Key key) const
{
    return Pressed(key);
}

bool Keyboard::KeyReleased(const Key key) const
{
    return Released(key);
}

bool Keyboard::KeyDown(const Key key) const
{
    return glfwGetKey(handle, key);
}