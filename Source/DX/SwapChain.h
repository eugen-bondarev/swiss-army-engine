#ifndef __DX_SwapChain_h__
#define __DX_SwapChain_h__

#pragma once

#include "../Graphics/SwapChainBase.h"
#include "Common.h"

FORWARD_DECLARE(Window);

namespace DX
{
    class SwapChain : public Base::SwapChain
    {
    friend class Instance;

    public:
        SwapChain(const Window& Wnd);
       ~SwapChain() = default;

        void Present(const UINT SyncInterval = 1u, const UINT Flags = 0u);

        void Resize(const unsigned int Width = 0, const unsigned int Height = 0) override;

        IDXGISwapChain* GetSwapChain();

    private:
        ComPtr<IDXGISwapChain> DXSwapChain;
    };
}

#endif