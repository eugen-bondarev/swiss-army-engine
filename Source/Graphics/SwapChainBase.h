#ifndef __SwapChainBase_h__
#define __SwapChainBase_h__

#pragma once

namespace Base {

class SwapChain
{
public:
    SwapChain() = default;
   ~SwapChain() = default;

    virtual void Resize(const unsigned int Width, const unsigned int Height) = 0;
};

}

#endif