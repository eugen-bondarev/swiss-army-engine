#ifndef __DX_SwapChain_h__
#define __DX_SwapChain_h__

#pragma once

#include "../Graphics/SwapChainBase.h"
#include "Common.h"

namespace DX {

class SwapChain : public Base::SwapChain
{
friend class Instance;

public:
    SwapChain() = default;
   ~SwapChain() = default;

    void Present(const UINT SyncInterval = 1u, const UINT Flags = 0u);

    void Resize(const unsigned int Width, const unsigned int Height) override;

    IDXGISwapChain* GetSwapChain();
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

private:
    unsigned int Width;
    unsigned int Height;

    ComPtr<IDXGISwapChain>     DXSwapChain;
};

}

#endif