#include "RenderTargetView.h"

#include "Device/Device.h"

namespace DX 
{
    RenderTargetView::RenderTargetView(SwapChain* swapChain, const bool initDepth, Device& device) : device{device}
    {
        ComPtr<ID3D11Resource> backBuffer{nullptr};
        DX_TRY(swapChain->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
        Init(swapChain->GetSize(), backBuffer.Get(), initDepth);
    }

    RenderTargetView::RenderTargetView(const Vec2ui size, const bool initDepth, Device& device) : device{device}
    {
        Init(size, nullptr, initDepth);
    }

    void RenderTargetView::Init(const Vec2ui size, ID3D11Resource* resource, const bool initDepth)
    {
        if (!resource)
        {
            texture = CreatePtr<Texture>(size, nullptr, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
            DX_TRY(GetDevice().GetDxDevice().CreateRenderTargetView(texture->GetDXTexture(), nullptr, &dxRenderTargetView));
        }
        else
        {
            DX_TRY(GetDevice().GetDxDevice().CreateRenderTargetView(resource, nullptr, &dxRenderTargetView));
        }

        if (initDepth)
        {
            depthBuffer = CreatePtr<DepthBuffer>(size);
        }
    }

    Texture* RenderTargetView::GetTexture()
    {
        return texture.get();
    }

    ID3D11RenderTargetView* RenderTargetView::GetDXRenderTarget()
    {
        return dxRenderTargetView.Get();
    }

    void RenderTargetView::Bind()
    {
        device.GetDxContext().OMSetRenderTargets(1u, dxRenderTargetView.GetAddressOf(), depthBuffer ? depthBuffer->GetDXDepthStencilView() : nullptr);
    }

    void RenderTargetView::Unbind()
    {
        device.GetDxContext().OMSetRenderTargets(1u, nullptr, nullptr);
    }

    void RenderTargetView::Clear(const std::array<float, 4>& clearColor)
    {    
        device.GetDxContext().ClearRenderTargetView(dxRenderTargetView.Get(), clearColor.data());

        if (depthBuffer)
        {
            depthBuffer->Clear();
        }
    }

}