#include "TextureLibrary.h"

Engine::TextureLibrary::TextureLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context)
{
	m_device = device;
	m_context = context;
	m_activeTexture = "";
	m_samplerState = nullptr;
}

bool Engine::TextureLibrary::Initialize(const std::string diffuse, const std::string normal,
	const std::string emissive, const std::string roughness, const std::string metallic)
{
	m_textures.Emplace(m_device, m_context);
	auto texture = m_textures.At(0);
	if (!texture->Initialize(diffuse, normal, emissive, roughness, metallic))
		return false;

	CreateSamplerSate();
	texture->SetAsActive();
	return true;
}

bool Engine::TextureLibrary::CreateTexture(const std::string uniqueName, const std::string diffuse, 
	const std::string normal, const std::string emissive, const std::string roughness, const std::string metallic)
{
	if (m_device)
	{
		auto result = m_textureCodex.find(uniqueName);
		if (result == m_textureCodex.end())
		{
			auto ref = m_textures.Emplace(m_device, m_context);
			m_textureCodex.emplace(uniqueName, ref);
			auto newTexture = m_textures.At(ref);
			if (!newTexture->Initialize(diffuse, normal, emissive, roughness, metallic))
			{
				OpenDialog(L"Texture init failed!", To_wstr(std::string("Owner: ") + uniqueName).c_str());
				return false;
			}
			return true;
		}
	}
	return false;
}

void Engine::TextureLibrary::DeleteTexture(const std::string & uniqueName)
{
	auto found = m_textureCodex.find(uniqueName);
	if (found != m_textureCodex.end())
	{
		m_textures.Remove(found->second);
	}
}

bool Engine::TextureLibrary::SetAsActive(const std::string& uniqueName)
{
	if (m_activeTexture.compare(uniqueName))
	{
		auto found = m_textureCodex.find(uniqueName);
		if (found != m_textureCodex.end())
		{
			m_activeTexture = uniqueName;
			m_textures.At(found->second)->SetAsActive();
			return true;
		}
		else if (!m_activeTexture.empty())
		{
			auto defaultTexture = m_textures.At(0);
			if (!defaultTexture)
				throw std::exception();

			m_activeTexture = "";
			defaultTexture->SetAsActive();
		}
		return false;
	}
	return true;
}

void Engine::TextureLibrary::Clear()
{
	for (auto& x : m_textureCodex)
	{
		m_textures.Remove(x.second);
	}
	m_textureCodex.clear();
}

void Engine::TextureLibrary::CreateSamplerSate()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.MaxAnisotropy = 16;

	HRESULT result = m_device->CreateSamplerState(&sampDesc, &m_samplerState);
	CheckSuccess(result, L"Sampler State");
	m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}