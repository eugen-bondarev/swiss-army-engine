#ifndef __VK_Entities_SwapChain_SwapChain_h__
#define __VK_Entities_SwapChain_SwapChain_h__

#pragma once

#include "../../Common.h"

namespace VK
{
    class SwapChain
    {
    public:
        SwapChain();
       ~SwapChain();

    private:
        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;
    };
}

#endif