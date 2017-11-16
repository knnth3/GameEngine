#include "DX11RasterStateManager.h"

Graphics::DX11RasterStateManager::DX11RasterStateManager(ID3D11Device * device, ID3D11DeviceContext * context)
{
	m_device = device;
	m_context = context;
}

Graphics::DX11RasterStateManager::~DX11RasterStateManager()
{
	for (auto rasterState : m_RSStates)
	{
		if (rasterState.second)
		{
			rasterState.second->Release();
			rasterState.second = 0;
		}
	}
}

bool Graphics::DX11RasterStateManager::CreateRSS(const RSSSettings settings, std::string uniqueName)
{
	auto rss = m_RSStates.find(uniqueName);
	if (rss == m_RSStates.end())
	{
		//Create a null variable to fill out
		m_RSStates.emplace(uniqueName, nullptr);
		//Fill out desc
		D3D11_RASTERIZER_DESC desc;
		desc.AntialiasedLineEnable = settings.AntialiasedLineEnable;
		desc.CullMode = (D3D11_CULL_MODE)settings.CullMode;
		desc.DepthBias = settings.DepthBias;
		desc.DepthBiasClamp = settings.DepthBiasClamp;
		desc.DepthClipEnable = settings.DepthClipEnable;
		desc.FillMode = (D3D11_FILL_MODE)settings.FillMode;
		desc.FrontCounterClockwise = settings.FrontCounterClockwise;
		desc.MultisampleEnable = settings.MultisampleEnable;
		desc.ScissorEnable = settings.ScissorEnable;
		desc.SlopeScaledDepthBias = settings.SlopeScaledDepthBias;

		//Create RSS
		HRESULT result = m_device->CreateRasterizerState(&desc, &m_RSStates[uniqueName]);
		if (!FAILED(result))
			return true;
		else
			m_RSStates.erase(uniqueName);
	}
	return false;
}

bool Graphics::DX11RasterStateManager::SetActive(std::string uniqueName)
{
	auto rss = m_RSStates.find(uniqueName);
	if (rss != m_RSStates.end())
	{
		m_context->RSSetState(rss->second);
		return true;
	}
	return false;
}

bool Graphics::DX11RasterStateManager::RemoveRSS(std::string uniqueName)
{
	auto rss = m_RSStates.find(uniqueName);
	if (rss != m_RSStates.end())
	{
		rss->second->Release();
		m_RSStates.erase(uniqueName);
		return true;
	}
	return false;
}
