#include "ShaderLibrary.h"

using namespace Engine;

ShaderLibrary::ShaderLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context)
{
	m_device = device;
	m_context = context;
	m_activeShader = "";
}

bool ShaderLibrary::Initialize(const std::string& defaultVsPath, const std::string& defaultPsPath, 
	std::string* error, const std::string& fallbackVsPath, const std::string& fallbackPsPath)
{
	bool result = true;
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	auto ref = m_shaders.Emplace(m_device, m_context);
	auto shader = m_shaders.At(ref);
	std::string errorStr;
	if (shader)
	{
		if (!shader->SetVertexShader(defaultVsPath, errorStr))
		{
			result = shader->SetVertexShader(fallbackVsPath, errorStr);
		}

		if (result)
		{
			if (!shader->SetPixelShader(defaultPsPath, errorStr))
			{
				result = shader->SetPixelShader(fallbackPsPath, errorStr);
			}
		}

		if(result)
			shader->SetAsActive();
	}
	if (error && result)
	{
		error->insert(error->begin(), errorStr.begin(), errorStr.end());
	}

	return result;
}

const std::shared_ptr<DirectX_Shader> Engine::ShaderLibrary::GetShader(const std::string & uniqueName)
{
	int ref = 0;
	auto found = m_shaderCodex.find(uniqueName);
	if (found != m_shaderCodex.end())
	{
		auto entry = m_shaders.At(found->second);
		return entry;
	}
	return nullptr;
}

const std::shared_ptr<DirectX_Shader> ShaderLibrary::CreateShader(const std::string & uniqueName)
{
	int ref = 0;
	auto found = m_shaderCodex.find(uniqueName);
	if (found == m_shaderCodex.end())
	{
		ref = m_shaders.Emplace(m_device, m_context);
		m_shaderCodex.emplace(uniqueName, ref);
		return m_shaders.At(ref);
	}
	return nullptr;
}

void Engine::ShaderLibrary::DeleteShader(const std::string & uniqueName)
{
	auto found = m_shaderCodex.find(uniqueName);
	if (found != m_shaderCodex.end())
	{
		m_shaders.Remove(found->second);
	}
}

bool Engine::ShaderLibrary::SetAsActive(const std::string & uniqueName)
{
	if (m_activeShader.compare(uniqueName))
	{
		auto found = m_shaderCodex.find(uniqueName);
		if (found != m_shaderCodex.end())
		{
			m_activeShader = uniqueName;
			m_shaders.At(found->second)->SetAsActive();
			return true;
		}
		else if (!m_activeShader.empty())
		{
			auto defaultShader = m_shaders.At(0);
			if (!defaultShader)
				throw std::exception();

			m_activeShader = "";
			defaultShader->SetAsActive();
		}
		return false;
	}
	return true;
}

void Engine::ShaderLibrary::Clear()
{
	for (auto& x : m_shaderCodex)
	{
		m_shaders.Remove(x.second);
	}
	m_shaderCodex.clear();
}