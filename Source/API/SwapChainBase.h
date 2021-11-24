#ifndef __API_SwapChainBase_h__
#define __API_SwapChainBase_h__

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
        virtual void Resize(const Vec2ui size) = 0;

        Vec2ui GetSize() const;
        float GetAspectRatio() const;

    protected:
        RawWindow& window;
    };
}

#endif