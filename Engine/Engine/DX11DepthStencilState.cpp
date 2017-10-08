#include "DX11DepthStencilState.h"

#define Check(x, lpctstr) \
	if(!(x)) { MessageBox(0, lpctstr, L"Error", MB_OK);}

#define CheckSuccess(hresult) \
	{_com_error err(hresult); Check(SUCCEEDED(hresult), err.ErrorMessage());}

#define CLOSE_COM_PTR(ptr) \
	if(ptr) { ptr->Release(); ptr = nullptr;}


Lime::DX11DepthStencilState::DX11DepthStencilState(uint16_t windowWidth, uint16_t windowHeight, ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView* rtv)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_device = device;
	m_context = context;
	m_rtv = rtv;
	m_depthStencilBuffer = nullptr;
	m_depthStencilView = nullptr;
	m_3DdepthStencilState = nullptr;
	m_2DdepthStencilState = nullptr;

	ZeroMemory(&m_depthBufferDesc, sizeof(m_depthBufferDesc));
	m_depthBufferDesc.Width = m_windowWidth;
	m_depthBufferDesc.Height = m_windowHeight;
	m_depthBufferDesc.MipLevels = 1;
	m_depthBufferDesc.ArraySize = 1;
	m_depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_depthBufferDesc.SampleDesc.Count = 1;
	m_depthBufferDesc.SampleDesc.Quality = 0;
	m_depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_depthBufferDesc.CPUAccessFlags = 0;
	m_depthBufferDesc.MiscFlags = 0;

	//3D-Depth Enabled
	ZeroMemory(&m_3DdepthStencilDesc, sizeof(m_3DdepthStencilDesc));
	m_3DdepthStencilDesc.DepthEnable = true;
	m_3DdepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_3DdepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	m_3DdepthStencilDesc.StencilEnable = true;
	m_3DdepthStencilDesc.StencilReadMask = 0xFF;
	m_3DdepthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	m_3DdepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_3DdepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	m_3DdepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_3DdepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	m_3DdepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_3DdepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	m_3DdepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_3DdepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//2D-Depth Disabled
	ZeroMemory(&m_2DdepthStencilDesc, sizeof(m_2DdepthStencilDesc));
	m_2DdepthStencilDesc.DepthEnable = false;
	m_2DdepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_2DdepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	m_2DdepthStencilDesc.StencilEnable = true;
	m_2DdepthStencilDesc.StencilReadMask = 0xFF;
	m_2DdepthStencilDesc.StencilWriteMask = 0xFF;
	m_2DdepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_2DdepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	m_2DdepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_2DdepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_2DdepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_2DdepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	m_2DdepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_2DdepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	ZeroMemory(&m_depthStencilViewDesc, sizeof(m_depthStencilViewDesc));
	m_depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_depthStencilViewDesc.Texture2D.MipSlice = 0;

}

void Lime::DX11DepthStencilState::Initialize()
{
	HRESULT result;


	result = m_device->CreateDepthStencilState(&m_2DdepthStencilDesc, &m_2DdepthStencilState);
	CheckSuccess(result);

	result = m_device->CreateDepthStencilState(&m_3DdepthStencilDesc, &m_3DdepthStencilState);
	CheckSuccess(result);

	result = m_device->CreateTexture2D(&m_depthBufferDesc, NULL, &m_depthStencilBuffer);
	CheckSuccess(result);

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &m_depthStencilViewDesc, &m_depthStencilView);
	CheckSuccess(result);

	SetAsActive();
}

void Lime::DX11DepthStencilState::OnWindowResize(ID3D11RenderTargetView* rtv, uint16_t windowWidth, uint16_t windowHeight)
{
	m_rtv = rtv;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_depthBufferDesc.Width = m_windowWidth;
	m_depthBufferDesc.Height = m_windowHeight;
	Close();
	Initialize();
}

void Lime::DX11DepthStencilState::SetAsActive()
{
	m_context->OMSetRenderTargets(1, &m_rtv, m_depthStencilView);
	m_context->OMSetDepthStencilState(m_3DdepthStencilState, 1);
}

void Lime::DX11DepthStencilState::ClearView()
{
	m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);		
}

void Lime::DX11DepthStencilState::Close()
{

	CLOSE_COM_PTR(m_depthStencilBuffer);
	CLOSE_COM_PTR(m_depthStencilView);
	CLOSE_COM_PTR(m_2DdepthStencilState);
	CLOSE_COM_PTR(m_3DdepthStencilState);
}

Lime::DX11DepthStencilState::~DX11DepthStencilState()
{
	Close();
}

void Lime::DX11DepthStencilState::SetDepthBufferStatus(const bool value)
{
	if(value)
		m_context->OMSetDepthStencilState(m_3DdepthStencilState, 1);
	else
		m_context->OMSetDepthStencilState(m_2DdepthStencilState, 1);
}
