#ifndef __D3D_DEPTH_STENCIL_BUFFER_H__
#define __D3D_DEPTH_STENCIL_BUFFER_H__

#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dx10math.h>

#include "../header.h"

class DepthStencilBuffer
{
friend class Device;

private:
    DepthStencilBuffer(ID3D11Device* device);
    ~DepthStencilBuffer();

	ID3D11Texture2D* texture;
	ID3D11DepthStencilState* state;
	ID3D11DepthStencilView* view;

    DepthStencilBuffer(const DepthStencilBuffer&) = delete;
    DepthStencilBuffer& operator=(const DepthStencilBuffer&) = delete;
};

#endif