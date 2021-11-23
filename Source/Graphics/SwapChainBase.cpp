#include "../Window/RawWindow.h"
#include "SwapChainBase.h"

namespace Base
{
    SwapChain::SwapChain(const RawWindow& window) : window{window}
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