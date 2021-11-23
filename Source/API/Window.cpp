#include "Window.h"

namespace API
{
    GraphicsContext* Window::GetGraphicsContext()
    {
        return graphicsContext.get();
    }
}