#include "device.h"
#include "../window.h"

#include <stdexcept>

Device* gpu;

void create_device()
{
    gpu = new Device();
}

void destroy_device()
{
    delete gpu;
}

Device::Device()
{    
	HRESULT result;

    // Create a DirectX graphics interface factory.
	IDXGIFactory* factory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	CHECK_RESULT(result, "Failed to create factory.");

	// Use the factory to create an adapter for the primary graphics interface (video card).
	IDXGIAdapter* adapter;
	result = factory->EnumAdapters(0, &adapter);
	CHECK_RESULT(result, "Failed to create adapter.");

	// Enumerate the primary adapter output (monitor).
	IDXGIOutput* adapter_output;
	result = adapter->EnumOutputs(0, &adapter_output);
	CHECK_RESULT(result, "Failed to enumerate monitors.");

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
    uint32_t num_modes{0};
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
	CHECK_RESULT(result, "Failed to enumerate modes.");

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	// displayModeList = new DXGI_MODE_DESC[numModes];
    std::vector<DXGI_MODE_DESC> display_mode_list(num_modes);

	// Now fill the display mode list structures.
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list.data());
	CHECK_RESULT(result, "Failed to get display mode list.");

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
    uint32_t numerator{0}, denominator{0};
	for (uint32_t i = 0; i < num_modes; ++i)
	{
		if (display_mode_list[i].Width == static_cast<unsigned int>(window->get_width()))
		{
			if (display_mode_list[i].Height == static_cast<unsigned int>(window->get_height()))
			{
				numerator = display_mode_list[i].RefreshRate.Numerator;
				denominator = display_mode_list[i].RefreshRate.Denominator;
			}
		}
	}	
    
    // Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapter_desc;
	result = adapter->GetDesc(&adapter_desc);
	CHECK_RESULT(result, "Failed to acquire the GPU's description.");

	// Store the dedicated video card memory in megabytes.
	gpu_memory = static_cast<int>(adapter_desc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
    gpu_description.resize(128);
    size_t string_length{0};
	int error{wcstombs_s(&string_length, gpu_description.data(), gpu_description.size(), adapter_desc.Description, gpu_description.size())};
	if (error != 0)
	{
        throw std::runtime_error("Failed to acquire the GPU's name.");
	}

	adapter_output->Release();
	adapter->Release();
	factory->Release();

	DXGI_SWAP_CHAIN_DESC swap_chain_desc = create_structure<DXGI_SWAP_CHAIN_DESC>();

	// Set to a single back buffer.
	swap_chain_desc.BufferCount = 1;
	// Set the width and height of the back buffer.
	swap_chain_desc.BufferDesc.Width = static_cast<unsigned int>(window->get_width());
	swap_chain_desc.BufferDesc.Height = static_cast<unsigned int>(window->get_height());

	// Set regular 32-bit surface for the back buffer.
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Set the refresh rate of the back buffer.
	if (vsync_enabled)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swap_chain_desc.OutputWindow = window->get_handle();

	// Turn multisampling off.
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.Windowed = !window->is_in_fullscreen();

	// Set the scan line ordering and scaling to unspecified.
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swap_chain_desc.Flags = 0;

    // Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

    // Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(
        nullptr, 
        D3D_DRIVER_TYPE_HARDWARE, 
        nullptr, 
        0, 
        &feature_level, 
        1, 
		D3D11_SDK_VERSION, 
        &swap_chain_desc, 
        &swap_chain, 
        &device, 
        nullptr, 
        &device_context
    );

	CHECK_RESULT(result, "Failed to create swapchain.");

    // Get the pointer to the back buffer.
	ID3D11Texture2D* back_buffer;
	result = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	CHECK_RESULT(result, "Failed to get the pointer to the back buffer.");

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
	CHECK_RESULT(result, "Failed to create the render target view.");

	// Release pointer to the back buffer as we no longer need it.
	back_buffer->Release();

    depth_stencil_buffer = new DepthStencilBuffer(device);    
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_buffer->view);

    // Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = device->CreateRasterizerState(&rasterDesc, &raster_state);
	CHECK_RESULT(result, "Failed to create the rasterizer state.");

	// Now set the rasterizer state.
	device_context->RSSetState(raster_state);

    // Setup the viewport for rendering.
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(window->get_width());
	viewport.Height = static_cast<float>(window->get_height());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	device_context->RSSetViewports(1, &viewport);

    // Setup the projection matrix.
	float field_of_view, aspect_ratio;
	field_of_view = static_cast<float>(D3DX_PI) / 4.0f;
	aspect_ratio = static_cast<float>(window->get_width()) / static_cast<float>(window->get_height());

    constexpr static float screen_near = 0.1f;
    constexpr static float screen_far = 1000.0f;

	D3DXMatrixPerspectiveFovLH(&projection_mat, field_of_view, aspect_ratio, screen_near, screen_far);
	D3DXMatrixIdentity(&world_mat);
	D3DXMatrixOrthoLH(&ortho_mat, static_cast<float>(window->get_width()), static_cast<float>(window->get_height()), screen_near, screen_far);
}

Device::~Device()
{
    swap_chain->SetFullscreenState(false, NULL);

    raster_state->Release();

    delete depth_stencil_buffer;

    render_target_view->Release();

    device_context->Release();
    device->Release();

    swap_chain->Release();
}

void Device::begin_scene(const std::array<float, 4>& p_color)
{    
	// Clear the back buffer.
	device_context->ClearRenderTargetView(render_target_view, p_color.data());
    
	// Clear the depth buffer.
	device_context->ClearDepthStencilView(depth_stencil_buffer->view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::end_scene()
{
    swap_chain->Present(static_cast<int>(vsync_enabled), 0);
}