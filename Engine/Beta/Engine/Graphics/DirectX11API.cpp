#include "DirectX11API.h"
#include "DX11TextureManager.h"


Graphics::DirectX11API::DirectX11API()
{
	m_nOfBuffers = 2;
}

Graphics::DirectX11API::~DirectX11API()
{
}

bool Graphics::DirectX11API::Initialize(void* handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	HWND hwnd = (HWND)handle;

	// Store the vsync setting.
	m_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
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
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	char videoCardDescription[128];
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}
	m_videoCardDescription = videoCardDescription;

	// Release the display mode list.
	delete[] displayModeList;
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

	// Set to double buffer
	swapChainDesc.BufferCount = m_nOfBuffers;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
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
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

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
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = CreateRTV();
	if (FAILED(result))
	{
		return false;
	}

	// Create the depth stencil view.
	bool success = CreateDepthStenciState(screenWidth, screenHeight);
	if (!success)
	{
		return false;
	}

	// Create the rasterizer state.
	CreateRSS();

	result = CreateBlendState();
	if (FAILED(result))
	{
		return false;
	}
	//Create a Manager for all shaders
	CreateShaderManager();

	//Initialize Texture Manager
	DX11TextureManager::Initialize(m_device, m_deviceContext);

	//Create a Manager for all buffers
	CreateBufferManager();

	// Create the viewport.
	SetViewPort(screenWidth, screenHeight);


	return true;
}

void Graphics::DirectX11API::BeginScene()
{

	// Clear the depth buffer.
	m_depthStencilState->ClearView();
}

void Graphics::DirectX11API::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}
}

void Graphics::DirectX11API::Close()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (Transparency)
	{
		Transparency->Release();
		Transparency = 0;		
	}

	m_depthStencilState->Close();
	m_cbManager->Close();

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
}

void Graphics::DirectX11API::GetVideoCardInfo(std::string & name, int & memory)
{
	name = m_videoCardDescription;
	memory = m_videoCardMemory;
}

void Graphics::DirectX11API::ClearScreen(float r, float g, float b)
{
	float color[4];
	// Setup the color to clear the buffer to.
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = 1.0f;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
}

void Graphics::DirectX11API::SetWindowSize(uint16_t width, uint16_t height)
{
		HRESULT result;
		//Reset all objects that use window indexCount
		m_renderTargetView->Release();
		result = m_swapChain->ResizeBuffers(m_nOfBuffers, width, height, DXGI_FORMAT_UNKNOWN, NULL);
		CheckSuccess(result, L"Swap chain");
	
		CreateRTV();
		m_depthStencilState->OnWindowResize(m_renderTargetView, width, height);
		SetViewPort(width, height);
}

std::shared_ptr<Graphics::DX11BufferManager> Graphics::DirectX11API::GetBufferManager()
{
	return m_bufferManager;
}

std::shared_ptr<Graphics::DX11ShaderManager> Graphics::DirectX11API::GetShaderManager()
{
	return m_shaderManager;
}

std::shared_ptr<Graphics::DX11DepthStencilState> Graphics::DirectX11API::GetDepthStencilState()
{
	return m_depthStencilState;
}

std::shared_ptr<Graphics::DX11RasterStateManager> Graphics::DirectX11API::GetRasterStateManager()
{
	return m_RSSManager;
}

void Graphics::DirectX11API::CreateRSS()
{
	//Create the manager that will handle all raster state data
	m_RSSManager = std::make_shared<DX11RasterStateManager>(m_device, m_deviceContext);
}

HRESULT Graphics::DirectX11API::CreateRTV()
{
	HRESULT result;
	ID3D11Texture2D* backBufferPtr;

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (!FAILED(result))
	{
		// Create the render target view with the back buffer pointer.
		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;
	}

	return result;
}

bool Graphics::DirectX11API::CreateDepthStenciState(int screenWidth, int screenHeight)
{
	bool result;
	//Create the depth stencil state manager to handle all of the logic
	m_depthStencilState = std::make_unique<DX11DepthStencilState>(screenWidth, screenHeight, m_device, m_deviceContext, m_renderTargetView);
	result = m_depthStencilState->Initialize();
	return result;
}

void Graphics::DirectX11API::CreateBufferManager()
{
	m_cbManager = std::make_shared<DX11ConstantBuffer>(m_device, m_deviceContext);
	m_bufferManager = std::make_shared<DX11BufferManager>(m_device, m_deviceContext, m_cbManager);
}

void Graphics::DirectX11API::CreateShaderManager()
{
	m_shaderManager = std::make_shared<DX11ShaderManager>(m_device, m_deviceContext);
}

void Graphics::DirectX11API::SetViewPort(int screenWidth, int screenHeight)
{
	// Setup the viewport for rendering.
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);
}

HRESULT Graphics::DirectX11API::CreateBlendState()
{
	HRESULT result;
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	result = m_device->CreateBlendState(&blendDesc, &Transparency);
	CheckSuccess(result, L"Blend state");

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	return result;
}
