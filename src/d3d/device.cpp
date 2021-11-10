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
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
    size_t stringLength;
	unsigned int numModes, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

    bool vsync{true};

    // Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
        throw std::runtime_error("Failed to create factory.");
		// return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
        throw std::runtime_error("Failed to create adapter.");
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
        throw std::runtime_error("Failed to enumerate monitors.");
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
        throw std::runtime_error("Failed to enumerate modes.");
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
        throw std::runtime_error("Failed to get display mode list.");
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == static_cast<unsigned int>(window->get_width()))
		{
			if (displayModeList[i].Height == static_cast<unsigned int>(window->get_height()))
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}	
    
    // Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
        throw std::runtime_error("Failed to acquire the GPU's description.");
	}

	// Store the dedicated video card memory in megabytes.
	gpu_memory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, gpu_description, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
        throw std::runtime_error("Failed to acquire the GPU's name.");
	}

    // Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

    	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = static_cast<unsigned int>(window->get_width());
	swapChainDesc.BufferDesc.Height = static_cast<unsigned int>(window->get_height());

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    	// Set the refresh rate of the back buffer.
	if (vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = window->get_handle();

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
    swapChainDesc.Windowed = !window->is_in_fullscreen();

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

    	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

    // Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(
        nullptr, 
        D3D_DRIVER_TYPE_HARDWARE, 
        nullptr, 
        0, 
        &featureLevel, 
        1, 
		D3D11_SDK_VERSION, 
        &swapChainDesc, 
        &swap_chain, 
        &device, 
        nullptr, 
        &device_context
    );

	CHECK_RESULT(result, "Failed to create swapchain.");

    // Get the pointer to the back buffer.
	result = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	CHECK_RESULT(result, "Failed to get the pointer to the back buffer.");

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, nullptr, &render_target_view);
	CHECK_RESULT(result, "Failed to create the render target view.");

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();

    	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

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
	result = device->CreateTexture2D(&depthBufferDesc, nullptr, &depth_stencil_buffer);
	CHECK_RESULT(result, "Failed to create the texture for the depth buffer.");

    // Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
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
	result = device->CreateDepthStencilState(&depthStencilDesc, &depth_stencil_state);
    CHECK_RESULT(result, "Failed to create the depth stencil state.");

    // Set the depth stencil state.
	device_context->OMSetDepthStencilState(depth_stencil_state, 1);

    // Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(depth_stencil_buffer, &depthStencilViewDesc, &depth_stencil_view);
    CHECK_RESULT(result, "Failed to create the depth stencil view.");

    // Bind the render target view and depth stencil buffer to the output render pipeline.
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

    // Setup the raster description which will determine how and what polygons will be drawn.
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
	viewport.Width = static_cast<float>(window->get_width());
	viewport.Height = static_cast<float>(window->get_height());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	device_context->RSSetViewports(1, &viewport);

    // Setup the projection matrix.
	fieldOfView = static_cast<float>(D3DX_PI) / 4.0f;
	screenAspect = static_cast<float>(window->get_width()) / static_cast<float>(window->get_height());

    constexpr static float screen_near = 0.1f;
    constexpr static float screen_far = 1000.0f;

	D3DXMatrixPerspectiveFovLH(&projection_mat, fieldOfView, screenAspect, screen_near, screen_far);
	D3DXMatrixIdentity(&world_mat);
	D3DXMatrixOrthoLH(&ortho_mat, static_cast<float>(window->get_width()), static_cast<float>(window->get_height()), screen_near, screen_far);

    vsync_enabled = vsync;
}

Device::~Device()
{
    swap_chain->SetFullscreenState(false, NULL);

    raster_state->Release();

    depth_stencil_view->Release();
    depth_stencil_state->Release();
    depth_stencil_buffer->Release();

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
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::end_scene()
{
    swap_chain->Present(static_cast<int>(vsync_enabled), 0);
}