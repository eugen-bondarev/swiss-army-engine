#ifndef __ENGINE_WINDOW_H__
#define __ENGINE_WINDOW_H__

#pragma once

#include "header.h"

class Window
{
friend static LRESULT CALLBACK wnd_proc(HWND p_handle, UINT p_message, WPARAM p_wparam, LPARAM p_lparam);

public:
    using handle_t = HWND;

    Window(const int p_width, const int p_height, const std::string& p_app_name, const bool p_fullscreen);
    ~Window();

    void poll_events();

    bool is_running() const;
    bool is_in_fullscreen() const;
    int get_width() const;
    int get_height() const;
    handle_t get_handle();

private:
    LRESULT CALLBACK process_events(handle_t p_handle, UINT p_message, WPARAM p_wparam, LPARAM p_lparam);

    MSG msg;
    bool running{true};
    bool fullscreen{false};
    int width, height;
    std::string app_name{"Direct3D"};

    HINSTANCE instance;
    handle_t handle;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

extern Window* window;
void create_window();
void destroy_window();

#endif