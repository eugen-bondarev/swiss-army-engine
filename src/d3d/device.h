#ifndef __D3D_DEVICE_H__
#define __D3D_DEVICE_H__

#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dx10math.h>

#include "../header.h"

class Device
{
public:
    Device();
    ~Device();

    void begin_scene(const std::array<float, 4>& p_color = {1, 0, 0, 1});
    void end_scene();

private:    
	int gpu_memory;
	char gpu_description[128];

    bool vsync_enabled{true};

	IDXGISwapChain* swap_chain;
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	ID3D11RenderTargetView* render_target_view;
	ID3D11Texture2D* depth_stencil_buffer;
	ID3D11DepthStencilState* depth_stencil_state;
	ID3D11DepthStencilView* depth_stencil_view;
	ID3D11RasterizerState* raster_state;

	D3DXMATRIX projection_mat;
	D3DXMATRIX world_mat;
	D3DXMATRIX ortho_mat;

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;
};

extern Device* gpu;
void create_device();
void destroy_device();

#endif 