#include "DirectX_RSS.h"



using namespace Engine;

DirectX_RSS::DirectX_RSS(ID3D11Device3 * device, ID3D11DeviceContext3 * context, const std::string& name)
{
	m_device = device;
	m_context = context;
	m_name = name;
	m_rasterState = nullptr;
}

bool Engine::DirectX_RSS::Initialize(D3D11_RASTERIZER_DESC desc)
{
	//Create RSS
	HRESULT result = m_device->CreateRasterizerState(&desc, &m_rasterState);
	if (FAILED(result))
	{
		OpenDialog(L"CreateRasterizerState failed!", To_wstr(std::string("Owner: ") + m_name).c_str());
		return false;
	}
	return true;
}

void Engine::DirectX_RSS::SetAsActive()
{
	if(m_rasterState)
		m_context->RSSetState(m_rasterState.Get());
}
