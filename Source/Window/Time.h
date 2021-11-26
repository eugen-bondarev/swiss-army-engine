#ifndef __Window_Time_h__
#define __Window_Time_h__

#pragma once

#include "GLFWHeader.h"

class Time
{
public:
    Time() = default;
   ~Time() = default;

    void BeginFrame();
    void EndFrame();

    float GetDeltaTime() const;

private:
    float time {0};
    float deltaTime {0};
};

#endif