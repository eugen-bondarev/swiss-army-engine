#ifndef __Window_Events_Mouse_h__
#define __Window_Events_Mouse_h__

#pragma once

#include "IInputDevice.h"

using ButtonState = DefaultInputState;
using Button = DefaultInputEntity;

class Mouse : public IInputDevice<Button, ButtonState>
{
public:
    Mouse(GLFWwindow* handle);

    void ButtonCallback(int button, int action, int mods);
    bool ButtonPressed(const Button button) const;
    bool ButtonReleased(const Button button) const;
    bool ButtonDown(const Button button) const;
};

#endif