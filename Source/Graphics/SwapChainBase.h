#ifndef __Graphics_SwapChainBase_h__
#define __Graphics_SwapChainBase_h__

#pragma once

#include "../Common/Common.h"

FORWARD_DECLARE(CallbackManager);
FORWARD_DECLARE(RawWindow);

namespace Base
{
    class SwapChain
    {
    friend class ::CallbackManager;
    friend class ::RawWindow;

    public:
        SwapChain(RawWindow& window);
       ~SwapChain() = default;

        virtual void Present(const unsigned int syncInterval = 1u, const unsigned int flags = 0u) = 0;
        virtual void Resize(const unsigned int width = 0u, const unsigned int height = 0u) = 0;

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;

    protected:
        RawWindow& window;
    };
}

#endif