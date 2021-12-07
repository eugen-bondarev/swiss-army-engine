#ifndef __Window_Events_IInputDevice_h__
#define __Window_Events_IInputDevice_h__

#pragma once

#include "../GLFWHeader.h"
#include <map>

template <typename TEntity, typename TState>
class IInputDevice
{
public:
    using Map = std::map<TEntity, TState>;

    IInputDevice(GLFWwindow* handle) : handle {handle}
    {
    }

    virtual void EndFrame()
    {
        map.clear();
    }

protected:
    bool Pressed(const TEntity entity) const
    {
        return GetStateOf(entity) == TState::Pressed;
    }

    bool Released(const TEntity entity) const
    {
        return GetStateOf(entity) == TState::Released;
    }

    TState GetStateOf(const TEntity entity /* key or button */) const
    {
        const Map::const_iterator i {map.find(entity)};
        if (i == map.end()) { return TState::Idle; }
        return i->second;
    }

    GLFWwindow* handle;
    Map map;
};

enum class DefaultInputState
{
    Idle     = 0,
    Pressed  = 1,
    Released = 2,
};

using DefaultInputEntity = int;

#endif