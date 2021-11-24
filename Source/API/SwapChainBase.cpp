#include "../Window/RawWindow.h"
#include "SwapChainBase.h"

namespace Base
{
    SwapChain::SwapChain(RawWindow& window) : window{window}
    {
    }

    Vec2ui SwapChain::GetSize() const
    {
        return window.GetSize();
    }
}