#include "DX11Graphics.h"
#include <chrono>
#include <wchar.h>
#include <stdlib.h>
#include <stdlib.h>
#include "ModelLoader.h"

Lime::DX11Graphics::DX11Graphics(const HWND window, const uint32_t width, const uint32_t height):
	m_bufferCount(3)
{
	m_windowWidth = width;
	m_windowHeight = height;
	m_hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	HRESULT result = Initialize(window, width, height);
	CheckSuccess(result);
}

Lime::DX11Graphics::~DX11Graphics()
{
	Close();
}

HRESULT Lime::DX11Graphics::Initialize(const HWND window, const uint32_t width, const uint32_t height)
{
	HRESULT result;
	DXGI_MODE_DESC bufferDesc = { 0 };

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = m_bufferCount;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	CheckSuccess(result);
	//Create our BackBuffer
	CreateRTV();

	//Depth stencil state
	m_dsState = std::make_unique<DX11DepthStencilState>(m_windowWidth, m_windowHeight, m_device, m_deviceContext, m_renderTargetView);
	m_dsState->Initialize();

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
	CreateViewport(width, height);
	CreateBlendState();
	m_deviceContext->RSSetState(m_cullBack);

	return result;
}

void Lime::DX11Graphics::Close()
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

void Lime::DX11Graphics::RenderText(std::string text, std::shared_ptr<Model::Model3D>& model)
{
	if (model->m_texture == -1)
		TextureManager::SetDefaultActive(DefaultTextures::TEXT);
	else
		TextureManager::SetActive(model->m_texture);

	m_textShader->SetAsActive();
	for (auto x = 0; x < text.size(); x++)
	{
		Float8 textInfo;
		MatrixBuffer matrices;

		matrices.world = glm::transpose(model->GetModelMatrix());
		matrices.view = glm::transpose(m_camera->GetViewMatrix());
		matrices.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
		matrices.cameraPos = glm::vec3(0.0f);
		m_bufferManager->SetBufferData("Matrix", &matrices, Lime::ShaderType::Vertex);

		float currentElement = (float)x;
		float character = (float)text.at(x);
		float posOffset = 1.2f;
		textInfo.one = glm::vec4(currentElement, character, posOffset, 0.0f);
		textInfo.two = model->GetColor();
		m_3DmodelShader->SetConstBufferData("float8", &textInfo, Lime::ShaderType::Vertex);

		uint32_t size = (uint32_t)model->m_mesh->GetIndexCount();
		uint32_t vertOff = model->m_mesh->vertOffset;
		uint32_t indOff = model->m_mesh->indiciOffset;

		m_deviceContext->RSSetState(m_cullBack);
		m_bufferManager->DrawIndexed(size, indOff, vertOff);
	}
}

void Lime::DX11Graphics::RenderMesh(std::shared_ptr<Model::Model3D>& model)
{
	if (model->m_texture == -1)
		TextureManager::SetDefaultActive(DefaultTextures::MODEL);
	else
		TextureManager::SetActive(model->m_texture);

	m_3DmodelShader->SetAsActive();

	MatrixBuffer matrices;
	PF_PixelBuffer lightInfo;
	matrices.world = glm::transpose(model->GetModelMatrix());
	matrices.view = glm::transpose(m_camera->GetViewMatrix());
	matrices.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
	matrices.cameraPos = m_camera->GetPosition();
	matrices.color = model->GetColor();
	m_bufferManager->SetBufferData("Matrix", &matrices, Lime::ShaderType::Vertex);

	lightInfo.ambientColor = m_light.m_ambientColor;
	lightInfo.diffuseColor = m_light.m_diffuseColor;
	lightInfo.lightDirection = m_light.m_direction;
	lightInfo.specularColor = m_light.m_specularColor;
	lightInfo.specularPower = m_light.m_specularPower;
	m_3DmodelShader->SetConstBufferData("Light", &lightInfo, Lime::ShaderType::Pixel);

	uint32_t size = (uint32_t)model->m_mesh->GetIndexCount();
	uint32_t vertOff = model->m_mesh->vertOffset;
	uint32_t indOff = model->m_mesh->indiciOffset;

	m_deviceContext->RSSetState(m_cullBack);
	m_bufferManager->DrawIndexed(size, indOff, vertOff);
}

void Lime::DX11Graphics::Render2DMesh(std::shared_ptr<Model::Model2D>& model)
{
	if (model->m_texture == -1)
		TextureManager::SetDefaultActive(DefaultTextures::MODEL);
	else
		TextureManager::SetActive(model->m_texture);

	m_2DmodelShader->SetAsActive();
	MatrixBuffer matrices;
	matrices.world = glm::transpose(model->GetModelMatrix());
	matrices.view = glm::transpose(m_camera->GetIdentityMatrix());
	matrices.projection = glm::transpose(m_camera->Get2DProjectionMatrix());
	m_bufferManager->SetBufferData("Matrix", &matrices, Lime::ShaderType::Geometry);

	Float8 data;
	data.one = model->GetColor();
	data.two = { model->GetLength(), model->GetWidth(), 0.0f, 0.0f };
	m_bufferManager->SetBufferData("float8", &data, Lime::ShaderType::Geometry);

	uint32_t size = (uint32_t)model->m_mesh->GetIndexCount();
	uint32_t vertOff = model->m_mesh->vertOffset;
	uint32_t indOff = model->m_mesh->indiciOffset;

	m_deviceContext->RSSetState(m_cullBack);
	m_bufferManager->DrawIndexed(size, indOff, vertOff);
}

void Lime::DX11Graphics::RenderLine(std::shared_ptr<Model::Model3D>& model)
{
	TextureManager::SetDefaultActive(DefaultTextures::MODEL);
	m_lineShader->SetAsActive();

	MatrixBuffer matrices;
	matrices.world = glm::transpose(model->GetModelMatrix());
	matrices.view = glm::transpose(m_camera->GetViewMatrix());
	matrices.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
	matrices.color = model->GetColor();
	m_bufferManager->SetBufferData("Matrix", &matrices, Lime::ShaderType::Vertex);

	uint32_t size = (uint32_t)model->m_mesh->GetIndexCount();
	uint32_t vertOff = model->m_mesh->vertOffset;
	uint32_t indOff = model->m_mesh->indiciOffset;

	m_deviceContext->RSSetState(m_cullBack);
	m_bufferManager->DrawIndexed(size, indOff, vertOff);
}

bool Lime::DX11Graphics::Add3DModel(std::shared_ptr<Model::Model3D>& model)
{
	m_modelLib.Add3DModel(model);
	return true;
}

bool Lime::DX11Graphics::Add2DModel(std::shared_ptr<Model::Model2D>& model)
{
	m_modelLib.Add2DModel(model);
	return true;
}

bool Lime::DX11Graphics::Add3DLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color)
{
	auto mesh = Model::MeshLoader::CreateLine(pos1, pos2);
	std::shared_ptr<Model::Model3D> line = std::make_shared<Model::Model3D>(mesh);
	line->SetColor(color);
	line->m_meshType = Lime::Model::LINE;
	line->m_bDraw = true;
	m_modelLib.Add3DModel(line);
	return true;
}

bool Lime::DX11Graphics::AddText(std::string text, std::shared_ptr<TextController>& controller)
{
	auto ctrler = std::make_shared<TextController>(text);
	auto mesh = ctrler->GetInfo()->GetMesh();
	Add3DModel(mesh);
	controller = ctrler;
	return true;
}

void Lime::DX11Graphics::Draw()
{
	CreateBuffers();
	if (m_hasCreatedBuffers && m_camera != nullptr)
	{
		//Render 3D objects
		m_bufferManager->SetAsActive(BufferTypes::BUFFER_3D);
		m_dsState->SetDepthBufferStatus(true);
		for (size_t index = 0; index < m_modelLib.size(); index++)
		{
			if (m_modelLib[index]->m_bDraw == true)
			{
				m_modelLib[index]->m_bDraw = false;
				Model::MeshType type = m_modelLib[index]->m_meshType;
				if (type == Lime::Model::TEXT)
				{
					auto ptr = reinterpret_cast<TextInfo*>(m_modelLib[index]->m_ptr);
					RenderText(ptr->GetText(), m_modelLib[index]);
				}
				else if (type == Lime::Model::TRIANGLE)
				{
					RenderMesh(m_modelLib[index]);
				}
				else if (type == Lime::Model::LINE)
				{
					m_bufferManager->SetAsActive(BufferTypes::BUFFER_LINE);
					RenderLine(m_modelLib[index]);
					m_modelLib[index]->m_bDraw = true;
					m_bufferManager->SetAsActive(BufferTypes::BUFFER_3D);
				}
			}
		}

		//Render 2D objects
		m_bufferManager->SetAsActive(BufferTypes::BUFFER_2D);
		m_dsState->SetDepthBufferStatus(false);
		for (size_t index = 0; index < m_modelLib.size2D(); index++)
		{
			if (m_modelLib.at2D(index)->m_bDraw == true)
			{
				m_modelLib.at2D(index)->m_bDraw = false;
				Render2DMesh(m_modelLib.at2D(index));
			}
		}
	}
	m_swapChain->Present(0, 0);
	ResetView();
}

void Lime::DX11Graphics::ResizeWindow(const uint32_t width, const uint32_t height)
{
	HRESULT result;
	//Reset all objects that use window size
	m_renderTargetView->Release();
	result = m_swapChain->ResizeBuffers(m_bufferCount, width, height, DXGI_FORMAT_UNKNOWN, NULL);
	CheckSuccess(result);

	CreateRTV();
	m_dsState->OnWindowResize(m_renderTargetView, width, height);
	CreateViewport(width, height);
	//Is optional
	m_camera->SetResolution(width, height);
}

void Lime::DX11Graphics::Wireframe(bool statemnent)
{
	m_isWireframe = statemnent;
}

void Lime::DX11Graphics::ClearScreen(glm::vec3 color)
{
	//Clear our backbuffer to the updated two
	glm::vec4 bgColor(color, 1.0f);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, (float*)&bgColor);
}

void Lime::DX11Graphics::Reset()
{
	m_hasCreatedBuffers = false;
	m_modelLib.clear();
	Model::MeshLoader::Clear();
	Lime::TextureManager::Clear();
}

void Lime::DX11Graphics::AttatchCamera(std::shared_ptr<Camera>& ptr)
{
	m_camera = ptr;
}

void Lime::DX11Graphics::CreateBuffers()
{
	static uint32_t buffSize2D = 0;
	if (!m_modelLib.empty())
	{
		if (!m_hasCreatedBuffers)
		{
			m_bufferManager->AddVertexData(m_modelLib.VertexData(), sizeof(Model::Vertex), m_modelLib.VertexDataSize());
			m_bufferManager->AddIndexData(m_modelLib.IndexData(), sizeof(DWORD), m_modelLib.IndexDataSize());
			m_bufferManager->CompileVertexData(BufferTypes::BUFFER_GENERAL);
			m_hasCreatedBuffers = true;
		}
	}
}

void Lime::DX11Graphics::CreateConstBuffers()
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

HRESULT Lime::DX11Graphics::CreateRenderStates()
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

HRESULT Lime::DX11Graphics::CreateBlendState()
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

HRESULT Lime::DX11Graphics::CreateRTV()
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

void Lime::DX11Graphics::CreateViewport(const uint32_t width, const uint32_t height)
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

void Lime::DX11Graphics::SetZBufferStatus(const bool value)
{
	m_dsState->SetDepthBufferStatus(value);
}

void Lime::DX11Graphics::ResetView()
{
	m_dsState->ClearView();
	m_dsState->SetAsActive();
	m_deviceContext->RSSetViewports(1, &m_viewport);
}
