#include "DX11Graphics.h"
#include <chrono>
#include <wchar.h>
#include <stdlib.h>
#include <stdlib.h>

Lime::DX11Graphics::DX11Graphics(const HWND window, const UINT width, const UINT height):
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

HRESULT Lime::DX11Graphics::Initialize(const HWND window, const UINT width, const UINT height)
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
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &m_device, NULL, &m_deviceContext);
	CheckSuccess(result);
	//Create our BackBuffer
	CreateRTV();

	//Depth stencil state
	m_newDSState = std::make_unique<DX11DepthStencilState>(m_windowWidth, m_windowHeight, m_device, m_deviceContext, m_renderTargetView);
	m_newDSState->Initialize();

	//Shaders
	LPCWSTR vsPath = L"shaders/VertexShader.hlsl";
	LPCWSTR psPath = L"shaders/PixelShader.hlsl";
	m_newModelShader = std::make_unique<DX11Shader>(vsPath, psPath, m_device, m_deviceContext);
	m_newModelShader->Initialize();

	vsPath = L"shaders/FontVertexShader.hlsl";
	psPath = L"shaders/FontPixelShader.hlsl";
	m_newTextShader = std::make_unique<DX11Shader>(vsPath, psPath, m_device, m_deviceContext);
	m_newTextShader->Initialize();

	//Textures
	m_newTextTexture = std::make_unique<DX11Texture>(L"SpriteSheetx200.dds", m_device, m_deviceContext);
	m_newModelTexture = std::make_unique<DX11Texture>(L"outUV.dds", m_device, m_deviceContext);

	//BMBuffer Manager
	m_cbManager = std::make_shared<DX11ConstantBuffer>(m_device, m_deviceContext);
	m_newBufferManager = std::make_unique<DX11BufferManager>(m_device, m_deviceContext, m_cbManager);

	m_newModelShader->AttachConstBufferManager(m_cbManager);
	D3D11_BUFFER_DESC tbd = { 0 };
	tbd.Usage = D3D11_USAGE_DYNAMIC;
	tbd.ByteWidth = sizeof(PF_PixelBuffer);
	tbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tbd.MiscFlags = 0;
	tbd.StructureByteStride = 0;
	m_newModelShader->CreateConstantBuffer(tbd, "Light");

	m_newTextShader->AttachConstBufferManager(m_cbManager);
	D3D11_BUFFER_DESC txtbd = { 0 };
	txtbd.Usage = D3D11_USAGE_DYNAMIC;
	txtbd.ByteWidth = sizeof(TextBuffer);
	txtbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	txtbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	txtbd.MiscFlags = 0;
	txtbd.StructureByteStride = 0;
	m_newTextShader->CreateConstantBuffer(txtbd, "Text");

	RECT tex;
	tex.top = 0;
	tex.left = 0;
	tex.right = 1;
	tex.bottom = 1;
	HDC handle = (HDC)m_deviceContext;

	CreateConstBuffers();
	CreateRenderStates();
	CreateViewport(width, height);
	CreateBlendState();
	m_deviceContext->RSSetState(m_cullBack);

	return result;
}

void Lime::DX11Graphics::Close()
{
	SwapChain->Release();
	m_device->Release();
	m_deviceContext->Release();
	m_renderTargetView->Release();
	WireFrame->Release();
	m_cullBack->Release();
	Transparency->Release();
}

void Lime::DX11Graphics::RenderText(std::string text, std::shared_ptr<Model::Model3D> model)
{
	m_newTextTexture->SetAsActive();
	m_newTextShader->SetAsActive();
	for (auto x = 0; x < text.size(); x++)
	{
		TextBuffer textInfo;
		MatrixBuffer matrices;

		matrices.world = glm::transpose(model->GetModelMatrix());
		matrices.view = glm::transpose(m_camera->GetViewMatrix());
		matrices.projection = glm::transpose(m_camera->GetProjectionMatrix());
		matrices.cameraPos = glm::vec3(0.0f);
		m_newBufferManager->SetBufferData("Matrix", &matrices, Lime::ShaderType::Vertex);

		float currentElement = (float)x;
		float character = (float)text.at(x);
		float posOffset = 1.2f;
		textInfo.PosAscii = glm::vec4(currentElement, character, posOffset, 0.0f);
		textInfo.color = model->GetColor();
		m_newModelShader->SetConstBufferData("Text", &textInfo, Lime::ShaderType::Vertex);

		UINT size = (UINT)model->m_mesh->GetIndexCount();
		UINT vertOff = model->m_mesh->vertOffset;
		UINT indOff = model->m_mesh->indiciOffset;

		if (m_isWireframe)
		{
			m_deviceContext->RSSetState(WireFrame);
			m_newBufferManager->DrawIndexed(size, indOff, vertOff);
		}
		else
		{
			m_deviceContext->RSSetState(m_cullBack);
			m_newBufferManager->DrawIndexed(size, indOff, vertOff);
		}
	}
}

void Lime::DX11Graphics::RenderMesh(std::shared_ptr<Model::Model3D> model)
{
	m_newModelTexture->SetAsActive();
	m_newModelShader->SetAsActive();

	MatrixBuffer matrices;
	PF_PixelBuffer lightInfo;
	matrices.world = glm::transpose(model->GetModelMatrix());
	matrices.view = glm::transpose(m_camera->GetViewMatrix());
	matrices.projection = glm::transpose(m_camera->GetProjectionMatrix());
	matrices.cameraPos = m_camera->GetPosition();
	m_newBufferManager->SetBufferData("Matrix", &matrices, Lime::ShaderType::Vertex);

	lightInfo.ambientColor = m_light.m_ambientColor;
	lightInfo.diffuseColor = m_light.m_diffuseColor;
	lightInfo.lightDirection = m_light.m_direction;
	lightInfo.specularColor = m_light.m_specularColor;
	lightInfo.specularPower = m_light.m_specularPower;
	m_newModelShader->SetConstBufferData("Light", &lightInfo, Lime::ShaderType::Pixel);

	UINT size = (UINT)model->m_mesh->GetIndexCount();
	UINT vertOff = model->m_mesh->vertOffset;
	UINT indOff = model->m_mesh->indiciOffset;

	m_deviceContext->RSSetState(m_cullBack);
	m_newBufferManager->DrawIndexed(size, indOff, vertOff);
	//if (m_isWireframe)
	//{

	//}
	//else
	//{
	//	m_deviceContext->RSSetState(CWcullMode);
	//	m_deviceContext->DrawIndexed(size, indOff, vertOff);
	//	m_deviceContext->RSSetState(CCWcullMode);
	//	m_deviceContext->DrawIndexed(size, indOff, vertOff);
	//}
}

bool Lime::DX11Graphics::AddModel(std::shared_ptr<Model::Model3D>& model)
{
	m_newModelLib.AddModel(model);
	return true;
}

bool Lime::DX11Graphics::AddText(std::string text, std::shared_ptr<TextController>& controller)
{
	auto ctrler = std::make_shared<TextController>(text);
	auto mesh = ctrler->GetInfo()->GetMesh();
	AddModel(mesh);
	controller = ctrler;
	return true;
}

void Lime::DX11Graphics::Draw()
{
	static bool hasEntered = false;
	if (!m_newModelLib.empty())
	{
		if (!hasEntered)
		{
			CreateBuffers();
			hasEntered = true;
		}
	}
	if (hasEntered && m_camera != nullptr)
	{
		for (size_t index = 0; index < m_newModelLib.size(); index++)
		{
			Model::ModelType type = m_newModelLib[index]->modelType;
			Model::Texture tex = m_newModelLib[index]->GetTexture();
			if (type == Lime::Model::TEXT)
			{
				auto ptr = reinterpret_cast<TextInfo*>(m_newModelLib[index]->m_ptr);
				RenderText(ptr->GetText(), m_newModelLib[index]);
			}
			else if (type == Lime::Model::MESH)
			{
				RenderMesh(m_newModelLib[index]);
			}
		}
	}
	SwapChain->Present(0, 0);
}

void Lime::DX11Graphics::ResizeWindow(const UINT width, const UINT height)
{
	HRESULT result;
	//Reset all objects that use window size
	m_renderTargetView->Release();
	result = SwapChain->ResizeBuffers(m_bufferCount, width, height, DXGI_FORMAT_UNKNOWN, NULL);
	CheckSuccess(result);

	CreateRTV();
	m_newDSState->OnWindowResize(m_renderTargetView, width, height);
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
	//Clear our backbuffer to the updated color
	glm::vec4 bgColor(color, 1.0f);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, (float*)&bgColor);
	m_newDSState->ClearView();
	m_newDSState->SetAsActive();
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void Lime::DX11Graphics::Reset()
{
	m_newModelLib.clear();
}

void Lime::DX11Graphics::AttatchCamera(std::shared_ptr<Camera>& ptr)
{
	m_camera = ptr;
}

void Lime::DX11Graphics::CreateBuffers()
{
	m_newBufferManager->AddVertexData(m_newModelLib.VertexData(), sizeof(Model::Vertex), m_newModelLib.VertexDataSize());
	m_newBufferManager->AddIndexData(m_newModelLib.IndexData(), sizeof(DWORD), m_newModelLib.IndexDataSize());
	m_newBufferManager->CompileVertexData();
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
	m_newBufferManager->CreateBuffer(cbbd, "Matrix");
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
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	CheckSuccess(result);
	//Create our Render Target
	result = m_device->CreateRenderTargetView(BackBuffer, NULL, &m_renderTargetView);
	CheckSuccess(result);

	BackBuffer->Release();
	return result;
}

void Lime::DX11Graphics::CreateViewport(const UINT width, const UINT height)
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