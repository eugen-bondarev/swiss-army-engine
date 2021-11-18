#ifndef __Graphics_SwapChainBase_h__
#define __Graphics_SwapChainBase_h__

#pragma once

#include "../Common.h"

FORWARD_DECLARE(CallbackManager);
FORWARD_DECLARE(Window);

namespace Base
{
    class SwapChain
    {
        friend class ::CallbackManager;
        friend class ::Window;

    public:
        SwapChain(const Window& Wnd);
       ~SwapChain() = default;

        virtual void Resize(const unsigned int Width = 0, const unsigned int Height = 0) = 0;

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;

    protected:
        const Window& Wnd;
    };
}

#endif