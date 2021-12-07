#ifndef __Window_Events_Keyboard_h__
#define __Window_Events_Keyboard_h__

#pragma once

#include "IInputDevice.h"

using KeyState = DefaultInputState;
using Key = DefaultInputEntity;

class Keyboard : public IInputDevice<Key, KeyState>
{
public:
    Keyboard(GLFWwindow* handle);

    void KeyCallback(int key, int scancode, int action, int mods);
    bool KeyPressed(const Key key) const;
    bool KeyReleased(const Key key) const;
    bool KeyDown(const Key key) const;
};

#endif