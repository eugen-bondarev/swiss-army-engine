#ifndef __WINDOW_H__
#define __WINDOW_H__

#pragma once

#include "header.h"

class WindowClass
{
public:
    std::string GetName() noexcept;
    HINSTANCE GetInstance() noexcept;

    WindowClass() noexcept;
   ~WindowClass();

    static constexpr const char* CLASS_NAME = "WinClass";

private:
    WindowClass(const WindowClass&) = delete;
    WindowClass& operator=(const WindowClass&) = delete;    

private:
    HINSTANCE instance;
};

class Window
{
friend class WindowClass;

public:
    Window(const int width, const int height, const std::string& name);
   ~Window();

   HWND GetHandle();

private:
    static LRESULT CALLBACK HandleMsgSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    LRESULT HandleMsg(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    int width, height;
    HWND handle;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

#endif