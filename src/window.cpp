#include "window.h"

static WindowClass windowClass;

WindowClass::WindowClass() noexcept : instance{GetModuleHandle(nullptr)}
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = Window::HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);
}

WindowClass::~WindowClass()
{
    UnregisterClass(GetName().c_str(), GetInstance());
}

std::string WindowClass::GetName() noexcept
{
    return CLASS_NAME;
}

HINSTANCE WindowClass::GetInstance() noexcept
{
    return instance;
}

Window::Window(const int width, const int height, const std::string& name)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    if (FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
    {
        throw EXCEPTION();
    }

    handle = CreateWindow(
        WindowClass::CLASS_NAME,
        name.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, windowClass.GetInstance(), this
    );

    ShowWindow(handle, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(handle);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const window = static_cast<Window*>(create->lpCreateParams);

        SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

        return window->HandleMsg(handle, msg, wParam, lParam);
    }
    
    return DefWindowProc(handle, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));
    return window->HandleMsg(handle, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(handle, msg, wParam, lParam);
}

HWND Window::GetHandle()
{
    return handle;
}