#include "window.h"
#include "d3d/device.h"

int main()
{
    try
    {
        create_window();
        create_device();

        while (window->is_running())
        {
            window->poll_events();

            gpu->begin_scene();

            gpu->end_scene();
        }

        destroy_device();
        destroy_window();
    } 
    catch (const std::runtime_error& p_error)
    {
        LINE_OUT(p_error.what());
    }

    return 0;
}