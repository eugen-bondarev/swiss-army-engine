#ifndef __DX_SwapChain_h__
#define __DX_SwapChain_h__

#pragma once

#include "../API/SwapChainBase.h"
#include "Common.h"

FORWARD_DECLARE(RawWindow);

namespace DX
{
    class SwapChain : public Base::SwapChain
    {
    friend class GraphicsContext;

    public:
        SwapChain(RawWindow& window);
       ~SwapChain() = default;

        void Present(const unsigned int syncInterval = 1u, const unsigned int flags = 0u) override;
        void Resize(const Vec2ui size) override;

        IDXGISwapChain* GetSwapChain();

    private:
        ComPtr<IDXGISwapChain> dxSwapChain;
    };
}

#endif