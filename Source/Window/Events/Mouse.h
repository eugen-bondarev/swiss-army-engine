#ifndef __Window_Events_Mouse_h__
#define __Window_Events_Mouse_h__

#pragma once

#include "../../Common/Common.h"
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

    void EndFrame() override
    {
        IInputDevice::EndFrame();
        deltaPosition = {0.0f, 0.0f};
    }

    void PositionCallback(double x, double y)
    {
        const Vec2f lastPosition {position};
        position = Vec2f {static_cast<float>(x), static_cast<float>(y)};
        deltaPosition = position - lastPosition;
    }

    Vec2f GetPosition() const
    {
        return position;
    }

    Vec2f GetDeltaPosition() const
    {
        return deltaPosition;
    }

private:
    Vec2f position {0.0f};
    Vec2f deltaPosition {0.0f};
};

#endif