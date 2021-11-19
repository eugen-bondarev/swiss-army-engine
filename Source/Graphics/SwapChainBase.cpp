#include "../Window/Window.h"
#include "SwapChainBase.h"

namespace Base
{
    SwapChain::SwapChain(const Window &window) : window{window}
    {
    }

    unsigned int SwapChain::GetWidth() const
    {
        return window.GetWidth();
    }

    unsigned int SwapChain::GetHeight() const
    {
        return window.GetHeight();
    }
}