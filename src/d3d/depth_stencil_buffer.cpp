#include "depth_stencil_buffer.h"

#include "../window.h"

DepthStencilBuffer::DepthStencilBuffer(ID3D11Device* device)
{
    HRESULT result;

    // Initialize the description of the depth buffer.
    D3D11_TEXTURE2D_DESC depthBufferDesc = create_structure<D3D11_TEXTURE2D_DESC>();

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = static_cast<unsigned int>(window->get_width());
    depthBufferDesc.Height = static_cast<unsigned int>(window->get_height());
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    result = device->CreateTexture2D(&depthBufferDesc, nullptr, &texture);
    CHECK_RESULT(result, "Failed to create the texture for the depth buffer.");

    // Set up the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = create_structure<D3D11_DEPTH_STENCIL_DESC>();
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    result = device->CreateDepthStencilState(&depthStencilDesc, &state);
    CHECK_RESULT(result, "Failed to create the depth stencil state.");

    // Set up the depth stencil view description.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = create_structure<D3D11_DEPTH_STENCIL_VIEW_DESC>();
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    result = device->CreateDepthStencilView(texture, &depthStencilViewDesc, &view);
    CHECK_RESULT(result, "Failed to create the depth stencil view.");
}

DepthStencilBuffer::~DepthStencilBuffer()
{
    view->Release();
    state->Release();
    texture->Release();
}