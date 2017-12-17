#include "DX11ShaderManager.h"

Graphics::DX11ShaderManager::DX11ShaderManager(ID3D11Device3 * device, ID3D11DeviceContext3 * context)
{
	m_device = device;
	m_context = context;
	CreateBlendState();
}

Graphics::DX11ShaderManager::~DX11ShaderManager()
{
	m_shaders.clear();
	m_blendState.Reset();
}

void Graphics::DX11ShaderManager::CreateShader(std::string uniqueName, std::wstring path, std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout, const bool bHasGS)
{
	m_shaders[uniqueName] = std::make_unique<DX11Shader>(path.c_str(), m_device, m_context, bHasGS);

	if(!newLayout.empty())
		m_shaders[uniqueName]->SetLayout(newLayout);

	m_shaders[uniqueName]->Initialize();
}

void Graphics::DX11ShaderManager::DeleteShader(std::string uniqueName)
{
	m_shaders.erase(uniqueName);
}

bool Graphics::DX11ShaderManager::SetActive(std::string uniqueName)
{
	if (m_active.compare(uniqueName) == 0)
		return true;

	bool found = false;
	auto result = m_shaders.find(uniqueName);
	if (result != m_shaders.end())
	{
		found = true;
		result->second->SetAsActive();
		m_active = uniqueName;
	}
	return found;
}

bool Graphics::DX11ShaderManager::CreateBlendState()
{
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

	HRESULT result = m_device->CreateBlendState(&blendDesc, &m_blendState);
	if (FAILED(result))
	{
		CheckSuccess(result, L"Blend state");
		return false;
	}

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_context->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);

	return true;
}
