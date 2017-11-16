#include "DX11ShaderManager.h"

Graphics::DX11ShaderManager::DX11ShaderManager(ID3D11Device * device, ID3D11DeviceContext * context)
{
	m_device = device;
	m_context = context;
}

Graphics::DX11ShaderManager::~DX11ShaderManager()
{
	m_shaders.clear();
}

void Graphics::DX11ShaderManager::CreateShader(std::string uniqueName, std::wstring vsPath, std::wstring psPath, std::wstring gsPath, std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout)
{
	if (gsPath.empty())
		m_shaders[uniqueName] = std::make_unique<DX11Shader>(vsPath.c_str(), psPath.c_str(), m_device, m_context);
	else
		m_shaders[uniqueName] = std::make_unique<DX11Shader>(vsPath.c_str(), psPath.c_str(), m_device, m_context, gsPath.c_str());

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