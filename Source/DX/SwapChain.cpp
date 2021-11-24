#include "SwapChain.h"

#include "../Window/RawWindow.h"
#include "RenderTargetView.h"
#include "GraphicsContext.h"
#include "Device/Device.h"

namespace DX
{
    SwapChain::SwapChain(RawWindow& window) : Base::SwapChain(window)
    {
    }

    void SwapChain::Present(const unsigned int syncInterval, const unsigned int flags)
    {
        HRESULT hr = dxSwapChain->Present(syncInterval, flags);

        if (FAILED(hr))
        {
            if (hr == DXGI_ERROR_DEVICE_REMOVED)
            {
                throw EXCEPTION_WHAT(std::to_string(GetDevice().GetDxDevice().GetDeviceRemovedReason()));
            }
            else
            {
                throw EXCEPTION();
            }
        }
    }

    IDXGISwapChain* SwapChain::GetSwapChain()
    {
        return dxSwapChain.Get();
    }

    void SwapChain::Resize(const Vec2ui size)
    {
        dxSwapChain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
    }
}