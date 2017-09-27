#include "DX11Graphics.h"
#include <dxtex\DirectXTex.h>
#include <chrono>
#include <wchar.h>
#include <stdlib.h>
#include <stdlib.h>

Lime::DX11Graphics::DX11Graphics(const HWND window, const UINT width, const UINT height)
{
	m_hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	m_bufferCount = 3;
	m_hasBuffers = false;
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
	DXGI_MODE_DESC bufferDesc = {0};

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
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &m_dx11device, NULL, &m_dx11Context);
	CheckSuccess(result);
	//Create our BackBuffer
	CreateRTV();

	//Set our Render Target
	m_dx11Context->OMSetRenderTargets(1, &renderTargetView, NULL);
	LPCWSTR m_vsPath = L"shaders/VertexShader.hlsl";
	LPCWSTR m_psPath = L"shaders/PixelShader.hlsl";
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	CreateShaders(m_vsPath, m_psPath, layout, 2);

	RECT tex;
	tex.top = 0;
	tex.left = 0;
	tex.right = 1;
	tex.bottom = 1;
	HDC handle = (HDC)m_dx11Context;

	CreateConstBuffers();
	CreateRenderStates();
	CreateDepthStencil(width, height);
	CreateViewport(width, height);
	CreateBlendState();

	return result;
}

void Lime::DX11Graphics::Close()
{
	SwapChain->Release();
	m_dx11device->Release();
	m_dx11Context->Release();
	renderTargetView->Release();
	if (m_hasBuffers)
	{
		m_vertexBuffer->Release();
		m_indexBuffer->Release();
	}
	depthStencilBuffer->Release();
	m_depthStencilState->Release();
	m_depthStencilView->Release();
	m_ObjConstBuffer->Release();
	m_transparentBuffer->Release();
	m_textBuffer->Release();
	WireFrame->Release();
	if (!m_vertexShaders.empty())
	{
		for (size_t it = 0; it < m_vertexShaders.size(); it++)
		{
			m_vertexShaders[it]->Release();
		}
	}
	if (!m_pixelShaders.empty())
	{
		for (size_t it = 0; it < m_pixelShaders.size(); it++)
		{
			m_pixelShaders[it]->Release();
		}
	}
	if (!m_vertLayouts.empty())
	{
		for (size_t it = 0; it < m_vertLayouts.size(); it++)
		{
			m_vertLayouts[it]->Release();
		}
	}
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
	Transparency->Release();
	CCWcullMode->Release();
	CWcullMode->Release();
}

void Lime::DX11Graphics::AddModel(std::shared_ptr<Model2>& model)
{
	if (model->m_Data->renderType.compare("Triangle") == 0)
		m_models.push_back(model);

	static int VertCountOffset = 0;
	static int IndCountOffset = 0;
	static std::vector<UINT> cachedIDs;
	bool similar = false;
	for (auto const& id : cachedIDs)
	{
		if (id == model->m_Data->m_ObjectID)
		{
			similar = true;
		}
	}
	if (!similar)
	{
		cachedIDs.push_back(model->m_Data->m_ObjectID);
		model->m_Data->m_VertOffset = VertCountOffset;
		model->m_Data->m_IndiciOffset = IndCountOffset;
		VertCountOffset += (int)model->m_Data->m_Verticies.size();
		IndCountOffset += (int)model->m_Data->m_Indicies.size();

		m_modelLib.m_Verticies.insert(
			m_modelLib.m_Verticies.end(),
			std::make_move_iterator(model->m_Data->m_Verticies.begin()),
			std::make_move_iterator(model->m_Data->m_Verticies.end())
		);
		m_modelLib.m_Indicies.insert(
			m_modelLib.m_Indicies.end(),
			std::make_move_iterator(model->m_Data->m_Indicies.begin()),
			std::make_move_iterator(model->m_Data->m_Indicies.end())
		);
		CreateBuffers();
	}
	m_hasBuffers = true;
}

void Lime::DX11Graphics::DrawText(std::string text, std::shared_ptr<TextController>& controller)
{
	static bool isFirst = true;
	m_text.emplace_back(text);
	m_text.back().GetController(controller);
	if (isFirst)
	{
		std::shared_ptr<Model2> data = nullptr;
		m_text.back().GetData(data);
		AddModel(data);
	}
	m_hasBuffers = true;
}

void Lime::DX11Graphics::Draw()
{
	m_dx11Context->VSSetShader(m_vertexShaders[0], 0, 0);
	m_dx11Context->PSSetShader(m_pixelShaders[0], 0, 0);
	m_dx11Context->IASetInputLayout(m_vertLayouts[0]);
	if (m_hasBuffers && m_camera != nullptr)
	{
		std::multimap<float, std::shared_ptr<Model2>> tOrdering;
		for (auto model = 0; model < m_models.size(); model++)
		{
			glm::vec4 p(m_models[model]->GetPosition(), 1.0f);
			glm::vec4 res = m_models[model]->GetLocalToWorld() * p;
			glm::vec4 toCameraSpace = m_camera->GetProjectionMatrix() * res;
			//Uses the z plane for comparison since camera is static
			float len2 = toCameraSpace.z;
			tOrdering.insert(std::pair<float, std::shared_ptr<Model2>>(len2, m_models[model]));
		}
		for (auto model = tOrdering.rbegin(); model != tOrdering.rend(); ++model)
		{
			if (hasConsBuffers)
			{
				HRESULT result;
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				MatrixBuffer* dataPtr;
				TransparentBuffer* dataPtr2;
				result = m_dx11Context->Map(m_ObjConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				CheckSuccess(result);

				dataPtr = (MatrixBuffer*)mappedResource.pData;
				dataPtr->world = glm::transpose(model->second->GetLocalToWorld());
				dataPtr->view = glm::transpose(m_camera->GetViewMatrix());
				dataPtr->projection = glm::transpose(m_camera->GetProjectionMatrix());
				m_dx11Context->Unmap(m_ObjConstBuffer, 0);
				m_dx11Context->VSSetConstantBuffers(0, 1, &m_ObjConstBuffer);

				result = m_dx11Context->Map(m_transparentBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				CheckSuccess(result);

				dataPtr2 = (TransparentBuffer*)mappedResource.pData;
				dataPtr2->colorBlend = model->second->GetColor();
				m_dx11Context->Unmap(m_transparentBuffer, 0);
				m_dx11Context->PSSetConstantBuffers(0, 1, &m_transparentBuffer);
			}

			//Can be set to null if no state wanted
			//m_dx11Context->RSSetState(WireFrame);
			if (!m_textures.empty() && !m_samplerStates.empty())
			{
				m_dx11Context->PSSetShaderResources(0, 1, &m_textures[0]);
				m_dx11Context->PSSetSamplers(0, 1, &m_samplerStates[0]);
			}
			UINT size = model->second->m_Data->m_Indicies.size();
			UINT vertOff = model->second->m_Data->m_VertOffset;
			UINT indOff = model->second->m_Data->m_IndiciOffset;
			m_dx11Context->RSSetState(CCWcullMode);
			m_dx11Context->DrawIndexed(size, indOff, vertOff);
			m_dx11Context->RSSetState(CWcullMode);
			m_dx11Context->DrawIndexed(size, indOff, vertOff);
		}
		//Present the backbuffer to the screen
		SwapChain->Present(0, 0);
	}
}

ID3D11DeviceContext * Lime::DX11Graphics::GetDeviceContext() const
{
	return m_dx11Context;
}

ID3D11RenderTargetView * Lime::DX11Graphics::GetRenderTargetView() const
{
	return renderTargetView;
}

ID3D11DepthStencilView * Lime::DX11Graphics::GetDepthStencilView() const
{
	return m_depthStencilView;
}

D3D11_VIEWPORT Lime::DX11Graphics::GetScreenViewport() const
{
	return viewport;
}

void Lime::DX11Graphics::ResizeWindow(const UINT width, const UINT height)
{
	HRESULT result;
	//Reset all objects that use window size
	renderTargetView->Release();
	depthStencilBuffer->Release();
	m_depthStencilState->Release();
	m_depthStencilView->Release();
	result = SwapChain->ResizeBuffers(m_bufferCount, width, height, DXGI_FORMAT_UNKNOWN, NULL);
	CheckSuccess(result);

	CreateRTV();
	CreateDepthStencil(width, height);
	CreateViewport(width, height);
	m_camera->SetAspectRatio(width, height);
}

void Lime::DX11Graphics::AttatchCamera(std::shared_ptr<DX11Camera>& ptr)
{
	m_camera = ptr;
}

void Lime::DX11Graphics::LoadTextureFromFile(std::wstring filename)
{
	wchar_t ext[_MAX_EXT];
	errno_t err = _wsplitpath_s(filename.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
	HRESULT result;
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


HRESULT Lime::DX11Graphics::CreateBuffers()
{
	HRESULT result;

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex2) * (UINT)m_modelLib.m_Verticies.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * (UINT)m_modelLib.m_Indicies.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = m_modelLib.m_Verticies.data();
	result = m_dx11device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	CheckSuccess(result);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = m_modelLib.m_Indicies.data();
	m_dx11device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);

	UINT stride = sizeof(Vertex2);
	UINT offset = 0;
	m_dx11Context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_dx11Context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_dx11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return result;
}

HRESULT Lime::DX11Graphics::CreateShaders(LPCWSTR vsPath, LPCWSTR psPath, D3D11_INPUT_ELEMENT_DESC* layout, size_t layoutSize)
{
	HRESULT result;
	ID3DBlob *vsBlob = nullptr;
	ID3DBlob *psBlob = nullptr;
	ID3D11VertexShader* VS = nullptr;
	ID3D11PixelShader* PS = nullptr;
	ID3D11InputLayout* vertLayout = nullptr;

	result = CompileShader(vsPath, "main", "vs_5_0", &vsBlob);
	CheckSuccess(result);

	result = CompileShader(psPath, "main", "ps_5_0", &psBlob);
	CheckSuccess(result);

	result = m_dx11device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &VS);
	CheckSuccess(result);
	m_vertexShaders.push_back(VS);

	result = m_dx11device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &PS);
	CheckSuccess(result);
	m_pixelShaders.push_back(PS);

	result = m_dx11device->CreateInputLayout(layout, layoutSize, vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &vertLayout);
	CheckSuccess(result);
	m_vertLayouts.push_back(vertLayout);

	vsBlob->Release();
	psBlob->Release();

	return result;
}

HRESULT Lime::DX11Graphics::CreateConstBuffers()
{
	HRESULT result;
	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(MatrixBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	cbbd.StructureByteStride = 0;
	result = m_dx11device->CreateBuffer(&cbbd, NULL, &m_ObjConstBuffer);
	CheckSuccess(result);

	D3D11_BUFFER_DESC tbd = { 0 };
	tbd.Usage = D3D11_USAGE_DYNAMIC;
	tbd.ByteWidth = sizeof(TransparentBuffer);
	tbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tbd.MiscFlags = 0;
	tbd.StructureByteStride = 0;
	result = m_dx11device->CreateBuffer(&tbd, NULL, &m_transparentBuffer);
	CheckSuccess(result);

	D3D11_BUFFER_DESC txtbd = { 0 };
	txtbd.Usage = D3D11_USAGE_DYNAMIC;
	txtbd.ByteWidth = sizeof(TextBuffer);
	txtbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	txtbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	txtbd.MiscFlags = 0;
	txtbd.StructureByteStride = 0;
	result = m_dx11device->CreateBuffer(&txtbd, NULL, &m_textBuffer);
	CheckSuccess(result);

	hasConsBuffers = true;
	return result;
}

HRESULT Lime::DX11Graphics::CreateRenderStates()
{
	HRESULT result;
	D3D11_RASTERIZER_DESC wfdesc = {};
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	result = m_dx11device->CreateRasterizerState(&wfdesc, &WireFrame);
	CheckSuccess(result);

	return result;
}

HRESULT Lime::DX11Graphics::CreateDepthStencil(const UINT width, const UINT height)
{
	HRESULT result;
	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc = {0};
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
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

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = m_dx11device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	CheckSuccess(result);

	result = m_dx11device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	CheckSuccess(result);

	m_dx11Context->OMSetDepthStencilState(m_depthStencilState, 1);

	result = m_dx11device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	CheckSuccess(result);

	m_dx11Context->OMSetRenderTargets(1, &renderTargetView, m_depthStencilView);

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

	result = m_dx11device->CreateBlendState(&blendDesc, &Transparency);
	CheckSuccess(result);

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	result = m_dx11device->CreateRasterizerState(&cmdesc, &CCWcullMode);
	CheckSuccess(result);

	cmdesc.FrontCounterClockwise = false;
	result = m_dx11device->CreateRasterizerState(&cmdesc, &CWcullMode);
	CheckSuccess(result);


	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_dx11Context->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	return result;
}

HRESULT Lime::DX11Graphics::CreateRTV()
{
	HRESULT result;

	ID3D11Texture2D* BackBuffer;
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	CheckSuccess(result);
	//Create our Render Target
	result = m_dx11device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	CheckSuccess(result);

	BackBuffer->Release();
	return result;
}

void Lime::DX11Graphics::CreateViewport(const UINT width, const UINT height)
{
	viewport = { 0 };
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;

	//Set the Viewport
	m_dx11Context->RSSetViewports(1, &viewport);
}

XMMATRIX Lime::DX11Graphics::GlmToXM(glm::mat4 matrix)
{
	glm::mat4* c = &matrix;
	XMMATRIX* cast = reinterpret_cast<XMMATRIX*>(c);
	return XMMatrixTranspose(*cast);
}

XMVECTOR Lime::DX11Graphics::GlmToXM(glm::vec3 matrix)
{
	glm::vec4* c = &glm::vec4(matrix, 0.0f);
	XMVECTOR* cast = reinterpret_cast<XMVECTOR*>(c);
	return *cast;
}

HRESULT Lime::DX11Graphics::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob ** blob)
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
