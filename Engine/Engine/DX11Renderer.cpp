#include "DX11Renderer.h"
#include <dxtex\DirectXTex.h>
#include <chrono>
#include <wchar.h>
#include <stdlib.h>
#include <stdlib.h>

Lime::DX11Renderer::DX11Renderer(HWND window,  int width, int height)
{
	m_width = width;
	m_height = height;
	m_hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	m_window = window;
	m_vsPath = L"shaders/VertexShader.hlsl";
	m_psPath = L"shaders/PixelShader.hlsl";
	m_hasBuffers = false;
	HRESULT result = Initialize();
	CheckSuccess(result);
}


Lime::DX11Renderer::~DX11Renderer()
{
	Close();
}

HRESULT Lime::DX11Renderer::Initialize()
{
	HRESULT result;
	DXGI_MODE_DESC bufferDesc = {0};

	bufferDesc.Width = m_width;
	bufferDesc.Height = m_height;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = m_window;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &m_dx11device, NULL, &m_dx11Context);
	CheckSuccess(result);
	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	CheckSuccess(result);
	//Create our Render Target
	result = m_dx11device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	CheckSuccess(result);

	BackBuffer->Release();

	//Set our Render Target
	m_dx11Context->OMSetRenderTargets(1, &renderTargetView, NULL);

	CreateShaders();
	CreateConstBuffers();
	CreateRenderStates();
	CreateDepthStencil();
	CreateViewport();

	return result;
}

void Lime::DX11Renderer::Close()
{
	SwapChain->Release();
	m_dx11device->Release();
	m_dx11Context->Release();
	renderTargetView->Release();
	if (m_hasBuffers)
	{
		m_vertexBuffer->Release();
		m_indexBuffer->Release();
		vertLayout->Release();
	}
	depthStencilView->Release();
	depthStencilBuffer->Release();
	VS->Release();
	PS->Release();
	vsBlob->Release();
	psBlob->Release();
	m_ObjConstBuffer->Release();
	WireFrame->Release();
	if (!m_samplerStates.empty())
	{
		for (size_t it = 0; it < m_samplerStates.size(); it++)
		{
			m_samplerStates[it]->Release();
		}
	}
	if (!m_textures.empty())
	{
		for (size_t it = 0; it < m_textures.size(); it++)
		{
			m_textures[it]->Release();
		}
	}
}

void Lime::DX11Renderer::AddModel(std::shared_ptr<Model2>& model)
{
	m_models.push_back(model);
	CreateBuffers();
	m_hasBuffers = true;
}

void Lime::DX11Renderer::Draw()
{
	if (m_hasBuffers && m_camera != nullptr)
	{

		//Clear our backbuffer to the updated color
		const float bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_dx11Context->ClearRenderTargetView(renderTargetView, bgColor);
		m_dx11Context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//Set the WVP matrix and send it to the constant buffer in effect file
		XMMATRIX world = GlmToXM(m_models[0]->GetLocalToWorld());
		XMMATRIX view = GlmToXM(m_camera->GetViewMatrix());
		XMMATRIX proj = GlmToXM(m_camera->GetProjectionMatrix());
		XMMATRIX worldViewProj = XMMatrixTranspose(world*view*proj);
		m_ObjBuffer.WVP = worldViewProj;
		m_dx11Context->UpdateSubresource(m_ObjConstBuffer, 0, NULL, &m_ObjBuffer, 0, 0);
		m_dx11Context->VSSetConstantBuffers(0, 1, &m_ObjConstBuffer);

		//Can be set to null if no state wanted
		//m_dx11Context->RSSetState(WireFrame);
		if (!m_textures.empty() && !m_samplerStates.empty())
		{
			m_dx11Context->PSSetShaderResources(0, 1, &m_textures[0]);
			m_dx11Context->PSSetSamplers(0, 1, &m_samplerStates[0]);
		}

		m_dx11Context->DrawIndexed(36, 0, 0);

		//Present the backbuffer to the screen
		SwapChain->Present(0, 0);
	}
}

void Lime::DX11Renderer::AttatchCamera(std::shared_ptr<DX11Camera>& ptr)
{
	m_camera = ptr;
}

void Lime::DX11Renderer::LoadShaderFromFile(std::wstring filename)
{
	wchar_t ext[_MAX_EXT];
	errno_t err = _wsplitpath_s(filename.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
	HRESULT result;
	TexMetadata info;
	ScratchImage image;
	if (_wcsicmp(ext, L".dds") == 0)
	{
		result = LoadFromDDSFile(filename.c_str(), DDS_FLAGS_NONE, nullptr, image);
		CheckSuccess(result);
	}
	if (SUCCEEDED(result))
	{
		ID3D11ShaderResourceView* pResource = nullptr;
		result = CreateShaderResourceView(m_dx11device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &pResource);
		CheckSuccess(result);

		m_textures.push_back(pResource);
		ID3D11SamplerState* state;
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		result = m_dx11device->CreateSamplerState(&sampDesc, &state);
		m_samplerStates.push_back(state);
	}
	CheckSuccess(result);
}


HRESULT Lime::DX11Renderer::CreateBuffers()
{
	HRESULT result;

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex2) * (UINT)m_models[0]->m_Data->m_Verticies.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * (UINT)m_models[0]->m_Data->m_Indicies.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = m_models[0]->m_Data->m_Verticies.data();
	result = m_dx11device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	CheckSuccess(result);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = m_models[0]->m_Data->m_Indicies.data();
	m_dx11device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);

	UINT stride = sizeof(Vertex2);
	UINT offset = 0;
	m_dx11Context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_dx11Context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	m_dx11device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &vertLayout);

	m_dx11Context->IASetInputLayout(vertLayout);

	m_dx11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return result;
}

HRESULT Lime::DX11Renderer::CreateShaders()
{
	HRESULT result;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;


	result = CompileShader(m_vsPath, "main", "vs_5_0", &vsBlob);
	CheckSuccess(result);

	result = CompileShader(m_psPath, "main", "ps_5_0", &psBlob);
	CheckSuccess(result);

	result = m_dx11device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &VS);
	CheckSuccess(result);

	result = m_dx11device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &PS);
	CheckSuccess(result);

	m_dx11Context->VSSetShader(VS, 0, 0);
	m_dx11Context->PSSetShader(PS, 0, 0);

	return result;
}

HRESULT Lime::DX11Renderer::CreateConstBuffers()
{
	HRESULT result;
	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(m_ObjBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	result = m_dx11device->CreateBuffer(&cbbd, NULL, &m_ObjConstBuffer);
	CheckSuccess(result);

	return result;
}

HRESULT Lime::DX11Renderer::CreateRenderStates()
{
	HRESULT result;
	D3D11_RASTERIZER_DESC wfdesc = {};
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	result = m_dx11device->CreateRasterizerState(&wfdesc, &WireFrame);
	CheckSuccess(result);

	return result;
}

HRESULT Lime::DX11Renderer::CreateDepthStencil()
{
	HRESULT result;
	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = {0};
	depthStencilDesc.Width = m_width;
	depthStencilDesc.Height = m_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	result = m_dx11device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	CheckSuccess(result);

	result = m_dx11device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	CheckSuccess(result);

	m_dx11Context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	return result;
}

void Lime::DX11Renderer::CreateViewport()
{
	D3D11_VIEWPORT viewport = { 0 };

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)m_width;
	viewport.Height = (float)m_height;

	//Set the Viewport
	m_dx11Context->RSSetViewports(1, &viewport);
}

XMMATRIX Lime::DX11Renderer::GlmToXM(glm::mat4 matrix)
{
	glm::mat4* c = &matrix;
	XMMATRIX* cast = reinterpret_cast<XMMATRIX*>(c);
	return *cast;
}

HRESULT Lime::DX11Renderer::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob ** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}
