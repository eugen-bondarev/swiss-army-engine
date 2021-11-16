#ifndef __DX_SwapChain_h__
#define __DX_SwapChain_h__

#pragma once

#include "Common.h"

namespace DX {

class SwapChain
{
friend class Instance;

public:
    SwapChain() = default;
   ~SwapChain() = default;

    void Present(const UINT SyncInterval = 1u, const UINT Flags = 0u);

private:
    ComPtr<IDXGISwapChain>     DXSwapChain;
};

}

#endif