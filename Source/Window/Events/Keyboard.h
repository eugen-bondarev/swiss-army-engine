#ifndef __Window_Events_Keyboard_h__
#define __Window_Events_Keyboard_h__

#pragma once

#include "../GLFWHeader.h"
#include <vector>
#include <map>

enum class KeyState
{
    Idle     = 0,
    Pressed  = 1,
    Released = 2
};

using Key = int;

class Keyboard
{
public:
    using Map = std::map<Key, KeyState>;

    inline Keyboard(GLFWwindow* handle) : handle {handle}
    {
    }

    inline void KeyCallback(int key, int scancode, int action, int mods)
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

    inline bool KeyPressed(const Key key) const
    {
        return GetStateOf(key) == KeyState::Pressed;
    }

    inline bool KeyReleased(const Key key) const
    {
        return GetStateOf(key) == KeyState::Released;
    }

    inline bool KeyDown(const Key key) const
    {
        return glfwGetKey(handle, key);
    }

    inline KeyState GetStateOf(const Key key) const
    {
        const Map::const_iterator i = map.find(key);
        if (i == map.end()) { return KeyState::Idle; }
        return i->second;
    }

    inline void EndFrame()
    {
        map.clear();
    }

private:
    Map map;
    GLFWwindow* handle;
};

#endif