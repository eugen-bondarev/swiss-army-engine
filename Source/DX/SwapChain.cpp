#include "SwapChain.h"

#include "RenderTargetView.h"
#include "Instance.h"

namespace DX {

void SwapChain::Present(const UINT SyncInterval, const UINT Flags)
{
    HRESULT hr = DXSwapChain->Present(SyncInterval, Flags);   

    if (FAILED(hr))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw EXCEPTION_WHAT(std::to_string(DX::GetDevice()->GetDeviceRemovedReason()));
        }
        else
        {
            throw EXCEPTION();
        }
    }
}

IDXGISwapChain* SwapChain::GetSwapChain()
{
    return DXSwapChain.Get();
}

void SwapChain::Resize(const unsigned int Width, const unsigned int Height)
{
    DXSwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
}

}