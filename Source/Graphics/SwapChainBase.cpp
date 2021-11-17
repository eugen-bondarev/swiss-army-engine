#include "SwapChainBase.h"
namespace Base {

unsigned int SwapChain::GetWidth() const
{
    return Width;
}

unsigned int SwapChain::GetHeight() const
{
    return Height;
}

void SwapChain::SetWidth(const unsigned int NewWidth)
{
    Width = NewWidth;
}

void SwapChain::SetHeight(const unsigned int NewHeight)
{
    Height = NewHeight;
}

void SwapChain::SetSize(const unsigned int NewWidth, const unsigned int NewHeight)
{
    Width = NewWidth; Height = NewHeight;
}

}