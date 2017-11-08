#include "DX11Graphics.h"
#include <chrono>
#include <wchar.h>
#include <stdlib.h>
#include <stdlib.h>
#include "MeshLoader.h"

Graphics::DX11Graphics::DX11Graphics():
	m_bufferCount(3)
{
	m_hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	m_videoCardMemory = 0;
	m_videoCardDescription = "";
	m_vertexManager = std::make_shared<VertexManager>();
}

Graphics::DX11Graphics::~DX11Graphics()
{
}

bool Graphics::DX11Graphics::Initialize(void * handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen)
{
	m_windowWidth = screenWidth;
	m_windowHeight = screenHeight;

	bool status;
	DXGI_MODE_DESC bufferDesc = { 0 };

	bufferDesc.Width = m_windowWidth;
	bufferDesc.Height = m_windowHeight;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = m_bufferCount;
	swapChainDesc.OutputWindow = (HWND)handle;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (result != S_OK)
	{
		CheckSuccess(result);
		return false;
	}

	//Create our BackBuffer
	CreateRTV();

	//Depth stencil state
	m_depthStencilState = std::make_unique<DX11DepthStencilState>(m_windowWidth, m_windowHeight, m_device, m_deviceContext, m_renderTargetView);
	status = m_depthStencilState->Initialize();
	if (!status)
	{
		return false;
	}

	//Shaders
	LPCWSTR vsPath = L"EngineAssets/shaders/VertexShader.hlsl";
	LPCWSTR psPath = L"EngineAssets/shaders/PixelShader.hlsl";
	LPCWSTR gsPath = L"";
	m_3DmodelShader = std::make_unique<DX11Shader>(vsPath, psPath, m_device, m_deviceContext);
	m_3DmodelShader->Initialize();

	vsPath = L"EngineAssets/shaders/FontVertexShader.hlsl";
	psPath = L"EngineAssets/shaders/FontPixelShader.hlsl";
	m_textShader = std::make_unique<DX11Shader>(vsPath, psPath, m_device, m_deviceContext);
	m_textShader->Initialize();

	vsPath = L"EngineAssets/shaders/VertexShader2D.hlsl";
	psPath = L"EngineAssets/shaders/PixelShader2D.hlsl";
	gsPath = L"EngineAssets/shaders/GeometryShader2D.hlsl";
	m_2DmodelShader = std::make_unique<DX11Shader>(vsPath, psPath, m_device, m_deviceContext, gsPath);
	m_2DmodelShader->Initialize();

	vsPath = L"EngineAssets/shaders/VertexShader_Line.hlsl";
	psPath = L"EngineAssets/shaders/PixelShader_Line.hlsl";
	m_lineShader = std::make_unique<DX11Shader>(vsPath, psPath, m_device, m_deviceContext);
	m_lineShader->Initialize();

	//Textures
	TextureManager::Initialize(m_device, m_deviceContext);

	//Buffer Manager
	m_cbManager = std::make_shared<DX11ConstantBuffer>(m_device, m_deviceContext);
	m_bufferManager = std::make_unique<DX11BufferManager>(m_device, m_deviceContext, m_cbManager);

	m_3DmodelShader->AttachConstBufferManager(m_cbManager);
	D3D11_BUFFER_DESC tbd = { 0 };
	tbd.Usage = D3D11_USAGE_DYNAMIC;
	tbd.ByteWidth = sizeof(PF_PixelBuffer);
	tbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tbd.MiscFlags = 0;
	tbd.StructureByteStride = 0;
	m_3DmodelShader->CreateConstantBuffer(tbd, "Light");

	m_textShader->AttachConstBufferManager(m_cbManager);
	D3D11_BUFFER_DESC txtbd = { 0 };
	txtbd.Usage = D3D11_USAGE_DYNAMIC;
	txtbd.ByteWidth = sizeof(Float8);
	txtbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	txtbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	txtbd.MiscFlags = 0;
	txtbd.StructureByteStride = 0;
	m_textShader->CreateConstantBuffer(txtbd, "float8");

	CreateConstBuffers();
	CreateRenderStates();
	CreateViewport(m_windowWidth, m_windowHeight);
	CreateBlendState();
	m_deviceContext->RSSetState(m_cullBack);

	return status;
}

void Graphics::DX11Graphics::BeginScene()
{
	// Clear the depth buffer.
	m_depthStencilState->ClearView();
}

void Graphics::DX11Graphics::EndScene()
{
	this->Draw();
}

void Graphics::DX11Graphics::Close()
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}
	CLOSE_COM_PTR(m_device);
	CLOSE_COM_PTR(m_deviceContext);
	CLOSE_COM_PTR(m_swapChain);
	CLOSE_COM_PTR(m_renderTargetView);
	CLOSE_COM_PTR(WireFrame);
	CLOSE_COM_PTR(m_cullBack);
	CLOSE_COM_PTR(Transparency);
}

void Graphics::DX11Graphics::GetVideoCardInfo(std::string & name, int & memory)
{
	name = m_videoCardDescription;
	memory = m_videoCardMemory;
}

void Graphics::DX11Graphics::ClearScreen(float r, float g, float b)
{
	//Clear our backbuffer to the updated two
	glm::vec4 bgColor(r,g,b, 1.0f);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, (float*)&bgColor);
}

void Graphics::DX11Graphics::AddModel(std::shared_ptr<Model::Model3D>& model)
{
	int ID = model->GetUniqueID();
	auto result = m_cachedModels_3D.find(ID);
	if (result == m_cachedModels_3D.end())
	{
		VertexManagerInfo info;
		info.model = model;
		m_vertexManager->AddModel(info);
		m_cachedModels_3D.emplace(ID, info);
	}
	m_modelDrawCache_3D.push_back(ID);
}

void Graphics::DX11Graphics::Draw()
{
	//Recompute if new data is available
	if (m_vertexManager->HasNewData())
		FillBuffers();

	//Draw objects
	if (m_bFilled)
	{
		//Process 3D objects
		for (auto x : m_modelDrawCache_3D)
		{
			m_bufferManager->SetAsActive(BufferTypes::BUFFER_3D);
			m_depthStencilState->SetDepthBufferStatus(true);
			ProcessObject_3D(m_cachedModels_3D[x]);
		}
		m_modelDrawCache_3D.clear();
	}
	//Present to screen
	m_swapChain->Present(0, 0);
}

void Graphics::DX11Graphics::ProcessObject_3D(VertexManagerInfo& info)
{
	//Grab model info
	TextureID texture = info.model->GetTexture();
	MeshID mesh = info.model->GetMesh();

	//Activate relevant texture
	TextureManager::SetDefaultActive(DefaultTextures::MODEL);
	m_3DmodelShader->SetAsActive();

	//Add info Here
	MatrixBuffer matrices;
	PF_PixelBuffer lightInfo;
	matrices.world = glm::transpose(info.model->GetModelMatrix());
	matrices.view = glm::transpose(m_camera->GetViewMatrix());
	matrices.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
	matrices.cameraPos = m_camera->GetPosition();
	matrices.color = info.model->GetColor();
	m_bufferManager->SetBufferData("Matrix", &matrices, Graphics::ShaderType::Vertex);

	lightInfo.ambientColor = m_light.m_ambientColor;
	lightInfo.diffuseColor = m_light.m_diffuseColor;
	lightInfo.lightDirection = m_light.m_direction;
	lightInfo.specularColor = m_light.m_specularColor;
	lightInfo.specularPower = m_light.m_specularPower;
	m_bufferManager->SetBufferData("Light", &lightInfo, Graphics::ShaderType::Pixel);

	m_deviceContext->RSSetState(m_cullBack);
	m_bufferManager->DrawIndexed(info.size, info.vertexOffset, info.indexOffset);
}

void Graphics::DX11Graphics::ResizeWindow(const uint32_t width, const uint32_t height)
{
	HRESULT result;
	//Reset all objects that use window size
	m_renderTargetView->Release();
	result = m_swapChain->ResizeBuffers(m_bufferCount, width, height, DXGI_FORMAT_UNKNOWN, NULL);
	CheckSuccess(result);

	CreateRTV();
	m_depthStencilState->OnWindowResize(m_renderTargetView, width, height);
	CreateViewport(width, height);
	//Is optional
	m_camera->SetResolution(width, height);
}

void Graphics::DX11Graphics::Wireframe(bool statemnent)
{
	m_isWireframe = statemnent;
}

void Graphics::DX11Graphics::Reset()
{
	m_bFilled = false;
	m_vertexManager->Reset();
	m_modelDrawCache_3D.clear();
	m_cachedModels_3D.clear();
	Model::MeshLoader::Clear();
	Graphics::TextureManager::Clear();
}

void Graphics::DX11Graphics::AttatchCamera(std::shared_ptr<Camera>& ptr)
{
	m_camera = ptr;
}

void Graphics::DX11Graphics::FillBuffers()
{
	std::vector<Model::Vertex> vertices;
	std::vector<Model::Index> indices;
	m_vertexManager->GetBufferData(vertices, indices);
	if (!indices.empty())
	{
		m_bufferManager->AddVertexData(vertices.data(), sizeof(Model::Vertex), (const unsigned int)vertices.size());
		m_bufferManager->AddIndexData(indices.data(), sizeof(Model::Index), (const unsigned int)indices.size());
		m_bufferManager->CompileVertexData();
		m_bFilled = true;
	}

}

void Graphics::DX11Graphics::CreateConstBuffers()
{
	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(MatrixBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	cbbd.StructureByteStride = 0;
	m_bufferManager->CreateBuffer(cbbd, "Matrix");
}

HRESULT Graphics::DX11Graphics::CreateRenderStates()
{
	HRESULT result;
	D3D11_RASTERIZER_DESC wfdesc = {};
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	result = m_device->CreateRasterizerState(&wfdesc, &WireFrame);
	CheckSuccess(result);

	D3D11_RASTERIZER_DESC cmdesc = {};
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = false;
	result = m_device->CreateRasterizerState(&cmdesc, &m_cullBack);
	CheckSuccess(result);

	return result;
}

HRESULT Graphics::DX11Graphics::CreateBlendState()
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
	CheckSuccess(result);

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	return result;
}

HRESULT Graphics::DX11Graphics::CreateRTV()
{
	HRESULT result;

	ID3D11Texture2D* BackBuffer;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	CheckSuccess(result);
	//Create our Render Target
	result = m_device->CreateRenderTargetView(BackBuffer, NULL, &m_renderTargetView);
	CheckSuccess(result);

	BackBuffer->Release();
	return result;
}

void Graphics::DX11Graphics::CreateViewport(const uint32_t width, const uint32_t height)
{
	m_viewport = { 0 };
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = (float)width;
	m_viewport.Height = (float)height;

	//Set the Viewport
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void Graphics::DX11Graphics::SetZBufferStatus(const bool value)
{
	m_depthStencilState->SetDepthBufferStatus(value);
}
