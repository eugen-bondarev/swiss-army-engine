#ifndef __SwapChainBase_h__
#define __SwapChainBase_h__

#pragma once

class CallbackManager;
class Window;

namespace Base {

class SwapChain
{
friend class ::CallbackManager;
friend class ::Window;

public:
    SwapChain() = default;
   ~SwapChain() = default;

    virtual void Resize(const unsigned int Width = 0, const unsigned int Height = 0) = 0;

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

protected:
    unsigned int Width, Height;

private:
    void SetWidth(const unsigned int NewWidth);
    void SetHeight(const unsigned int NewHeight);
    void SetSize(const unsigned int NewWidth, const unsigned int NewHeight);
};

}

#endif