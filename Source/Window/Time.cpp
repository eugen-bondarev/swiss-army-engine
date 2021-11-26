#include "Time.h"

void Time::BeginFrame()
{
    const float currentTime {static_cast<float>(glfwGetTime())};
    deltaTime = currentTime - time;
    time = currentTime;
}

void Time::EndFrame()
{
}

float Time::GetDeltaTime() const
{
    return deltaTime;
}