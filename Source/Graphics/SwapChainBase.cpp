#include "../Window/Window.h"
#include "SwapChainBase.h"

namespace Base
{
    SwapChain::SwapChain(const Window &Wnd) : Wnd{Wnd}
    {
    }

    unsigned int SwapChain::GetWidth() const
    {
        return Wnd.GetWidth();
    }

    unsigned int SwapChain::GetHeight() const
    {
        return Wnd.GetHeight();
    }
}