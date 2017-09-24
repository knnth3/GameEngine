#include "DX11Renderer.h"
#include <chrono>

Lime::DX11Renderer::DX11Renderer(HWND window,  int width, int height)
{
	m_width = width;
	m_height = height;
	m_hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	m_window = window;
	m_vsPath = L"shaders/VertexShader.hlsl";
	m_psPath = L"shaders/PixelShader.hlsl";
}


Lime::DX11Renderer::~DX11Renderer()
{
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
	CreateBuffers();
	CreateConstBuffers();
	CreateRenderStates();

	return result;
}

void Lime::DX11Renderer::Close()
{
	SwapChain->Release();
	m_dx11device->Release();
	m_dx11Context->Release();
	renderTargetView->Release();
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
	VS->Release();
	PS->Release();
	vsBlob->Release();
	psBlob->Release();
	vertLayout->Release();
	depthStencilView->Release();
	depthStencilBuffer->Release();
	m_ObjConstBuffer->Release();
	WireFrame->Release();
}

void Lime::DX11Renderer::AddModel(Vertex2* verts, UINT vertLen, DWORD* ind, UINT indLen)
{
	v = std::vector<Vertex2>(verts, verts + vertLen);
	indices = std::vector<DWORD>(ind, ind + indLen);
}

void Lime::DX11Renderer::Render()
{

	//Test Code

	//Keep the cubes rotating
	rot += .0005f;
	if (rot > 6.28f)
		rot = 0.0f;

	//Reset cube1World
	cube1World = XMMatrixIdentity();
	//Define cube1's world space matrix
	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, rot);
	Translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);

	//Set cube1's world space using the transformations
	cube1World = Translation * Rotation;

	//Reset cube2World
	cube2World = XMMatrixIdentity();

	//Define cube2's world space matrix
	Rotation = XMMatrixRotationAxis(rotaxis, -rot);
	Scale = XMMatrixScaling(1.3f, 1.3f, 1.3f);

	//Set cube2's world space matrix
	cube2World = Rotation * Scale;

	//Clear our backbuffer to the updated color
	const float bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	m_dx11Context->ClearRenderTargetView(renderTargetView, bgColor);
	m_dx11Context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Set the WVP matrix and send it to the constant buffer in effect file
	m_ObjBuffer.WVP = XMMatrixTranspose(cube1World * m_camera->GetViewMatrix() * m_camera->GetProjectionMatrix());
	m_dx11Context->UpdateSubresource(m_ObjConstBuffer, 0, NULL, &m_ObjBuffer, 0, 0);
	m_dx11Context->VSSetConstantBuffers(0, 1, &m_ObjConstBuffer);

	//Draw the first cube
	m_dx11Context->RSSetState(WireFrame);
	m_dx11Context->DrawIndexed(36, 0, 0);

	m_ObjBuffer.WVP = XMMatrixTranspose(cube2World * m_camera->GetViewMatrix() * m_camera->GetProjectionMatrix());
	m_dx11Context->UpdateSubresource(m_ObjConstBuffer, 0, NULL, &m_ObjBuffer, 0, 0);
	m_dx11Context->VSSetConstantBuffers(0, 1, &m_ObjConstBuffer);

	//Draw the second cube
	m_dx11Context->RSSetState(NULL);
	m_dx11Context->DrawIndexed(36, 0, 0);

	//Present the backbuffer to the screen
	SwapChain->Present(0, 0);
}

void Lime::DX11Renderer::AttatchCamera(std::shared_ptr<DX11Camera>& ptr)
{
	m_camera = ptr;
}

HRESULT Lime::DX11Renderer::CreateBuffers()
{
	HRESULT result;

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
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

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex2) * (UINT)v.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * (UINT)indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v.data();
	result = m_dx11device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	CheckSuccess(result);

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = indices.data();
	m_dx11device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);

	UINT stride = sizeof(Vertex2);
	UINT offset = 0;
	m_dx11Context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_dx11Context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	m_dx11device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	m_dx11device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	m_dx11device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &vertLayout);

	m_dx11Context->IASetInputLayout(vertLayout);

	m_dx11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)m_width;
	viewport.Height = (float)m_height;

	//Set the Viewport
	m_dx11Context->RSSetViewports(1, &viewport);
	m_dx11Context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

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
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(m_ObjBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	result = m_dx11device->CreateBuffer(&cbbd, NULL, &m_ObjConstBuffer);

	return result;
}

HRESULT Lime::DX11Renderer::CreateRenderStates()
{
	HRESULT result;
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	result = m_dx11device->CreateRasterizerState(&wfdesc, &WireFrame);
	return result;
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
