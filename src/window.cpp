#include "window.h"

class WindowManager
{
public:
    using windows_t = std::map<HWND, Window*>;

    void add(const HWND p_handle, Window* p_window)
    {
        windows[p_handle] = p_window;
    }

    Window* get(const HWND p_handle)
    {
        return windows[p_handle];
    }

private:
    windows_t windows;
};

static WindowManager window_manager;

static LRESULT CALLBACK wnd_proc(HWND p_handle, UINT p_message, WPARAM p_wparam, LPARAM p_lparam)
{
    Window* window = window_manager.get(p_handle);

    if (p_message == WM_DESTROY || p_message == WM_CLOSE)
    {
        PostQuitMessage(0);
        return 0;
    }
    else
    {
        return window->process_events(p_handle, p_message, p_wparam, p_lparam);
    }
}

LRESULT CALLBACK Window::process_events(HWND p_handle, UINT p_message, WPARAM p_wparam, LPARAM p_lparam)
{
	switch (p_message)
	{
		case WM_KEYDOWN:
		{
			// m_Input->KeyDown((unsigned int)wparam);
            return 0;
		}

		case WM_KEYUP:
		{
			// m_Input->KeyUp((unsigned int)wparam);
            return 0;
		}

		default:
		{
			return DefWindowProc(p_handle, p_message, p_wparam, p_lparam);
		}
	}
}

Window::Window(const int p_width, const int p_height, const std::string& p_app_name, const bool p_fullscreen) : width{p_width}, height{p_height}, app_name{p_app_name}, fullscreen{p_fullscreen}
{
    WNDCLASSEX wnd_class;
    DEVMODE dev_mode;
    int pos_x, pos_y;

    instance = GetModuleHandle(NULL);

    wnd_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wnd_class.lpfnWndProc = wnd_proc;
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hInstance = instance;
    wnd_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wnd_class.hIconSm = wnd_class.hIcon;
    wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wnd_class.lpszMenuName = NULL;
    wnd_class.lpszClassName = app_name.c_str();
    wnd_class.cbSize = sizeof(WNDCLASSEX);
    
    RegisterClassEx(&wnd_class);

    // screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    // screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (fullscreen)
    {
        memset(&dev_mode, 0, sizeof(dev_mode));
        dev_mode.dmSize       = sizeof(dev_mode);
        dev_mode.dmPelsWidth  = static_cast<unsigned long>(p_width);
        dev_mode.dmPelsHeight = static_cast<unsigned long>(p_height);
        dev_mode.dmBitsPerPel = 32;			
        dev_mode.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        ChangeDisplaySettings(&dev_mode, CDS_FULLSCREEN);
        pos_x = pos_y = 0;
    }
    else
    {
        pos_x = (GetSystemMetrics(SM_CXSCREEN) - p_width)  / 2;
        pos_y = (GetSystemMetrics(SM_CYSCREEN) - p_height) / 2;
    }

    handle = CreateWindowEx(
        WS_EX_APPWINDOW, 
        app_name.c_str(), 
        app_name.c_str(), 
        !fullscreen ? WS_OVERLAPPEDWINDOW : WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        pos_x, pos_y, 
        p_width, p_height, 
        NULL, 
        NULL, 
        instance, 
        NULL
    );

    ShowWindow(handle, SW_SHOW);
    SetForegroundWindow(handle);
    SetFocus(handle);

    window_manager.add(handle, this);
    
	ZeroMemory(&msg, sizeof(MSG));
}

bool Window::is_running() const
{
    return running;
}

void Window::poll_events()
{
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT)
    {
        running = false;
    }
}

Window::handle_t Window::get_handle()
{
    return handle;
}

bool Window::is_in_fullscreen() const
{
    return fullscreen;
}

int Window::get_width() const
{
    return width;
}

int Window::get_height() const
{
    return height;
}

Window::~Window()
{
    ShowCursor(true);

    if(fullscreen)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    DestroyWindow(handle);
    UnregisterClass(app_name.c_str(), instance);    
}

Window* window;

void create_window()
{
    window = new Window(800, 600, "Hello, world!", false);
}

void destroy_window()
{
    delete window;
}