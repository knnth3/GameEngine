#include "RSSLibrary.h"

using namespace Engine;

RSSLibrary::RSSLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context) :
	m_default(device, context, "Default")
{
	m_device = device;
	m_context = context;
	m_activeRSS = "";
}

bool Engine::RSSLibrary::Initialize()
{
	bool value = false;
	D3D11_RASTERIZER_DESC desc = {};
	desc.AntialiasedLineEnable = true;
	desc.CullMode = D3D11_CULL_BACK;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = true;
	desc.MultisampleEnable = true;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	value = m_default.Initialize(desc);
	if (value)
		m_default.SetAsActive();

	return value;
}

void Engine::RSSLibrary::Clear()
{
	for (auto& x : m_rssCodex)
	{
		m_rss.Remove(x.second);
	}
	m_rssCodex.clear();
}

const std::shared_ptr<DirectX_RSS> Engine::RSSLibrary::GetRSS(const std::string & uniqueName)
{
	int ref = 0;
	auto found = m_rssCodex.find(uniqueName);
	if (found != m_rssCodex.end())
	{
		auto entry = m_rss.At(found->second);
		return entry;
	}
	return nullptr;
}

const std::shared_ptr<DirectX_RSS> Engine::RSSLibrary::CreateRSS(const std::string & uniqueName)
{
	int ref = 0;
	auto found = m_rssCodex.find(uniqueName);
	if (found == m_rssCodex.end())
	{
		ref = m_rss.Emplace(m_device, m_context, uniqueName);
		m_rssCodex.emplace(uniqueName, ref);
		return m_rss.At(ref);
	}
	return nullptr;
}

void Engine::RSSLibrary::DeleteRSS(const std::string & uniqueName)
{
	auto found = m_rssCodex.find(uniqueName);
	if (found != m_rssCodex.end())
	{
		m_rss.Remove(found->second);
	}
}

bool Engine::RSSLibrary::SetAsActive(const std::string & uniqueName)
{
	if (m_activeRSS.compare(uniqueName))
	{
		auto found = m_rssCodex.find(uniqueName);
		if (found != m_rssCodex.end())
		{
			m_activeRSS = uniqueName;
			m_rss.At(found->second)->SetAsActive();
			return true;
		}
		else if (!m_activeRSS.empty())
		{
			m_activeRSS = "";
			m_default.SetAsActive();
		}
		return false;
	}
	return true;
}
