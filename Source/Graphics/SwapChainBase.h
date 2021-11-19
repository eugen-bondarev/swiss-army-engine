#ifndef __Graphics_SwapChainBase_h__
#define __Graphics_SwapChainBase_h__

#pragma once

#include "../Common/Common.h"

FORWARD_DECLARE(CallbackManager);
FORWARD_DECLARE(Window);

namespace Base
{
    class SwapChain
    {
    friend class ::CallbackManager;
    friend class ::Window;

    public:
        SwapChain(const Window& window);
       ~SwapChain() = default;

        virtual void Present(const unsigned int syncInterval = 1u, const unsigned int flags = 0u) = 0;
        virtual void Resize(const unsigned int width = 0, const unsigned int height = 0) = 0;

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;

    protected:
        const Window& window;
    };
}

#endif