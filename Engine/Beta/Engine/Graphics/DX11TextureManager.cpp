#include "DX11TextureManager.h"
static Graphics::DX11TextureManager mngr;


Graphics::DX11TextureManager::DX11TextureManager()
{
	m_active = -1;
	m_device = nullptr;
	m_context = nullptr;
	m_defaultModelTexture = nullptr;
	m_defaultTextTexture = nullptr;
}

Graphics::TextureID Graphics::DX11TextureManager::CreateNewTexture(const std::string filePath)
{
	TextureID ID = -1;
	auto filepath = mngr.MultiByteToWideChar(filePath);
	if(!filepath.empty() && mngr.m_device)
	{
		auto result = mngr.m_filepaths.find(filepath);
		if (result == mngr.m_filepaths.end())
		{
			auto newTexture = std::make_unique<DX11Texture>(filepath.c_str(), mngr.m_device, mngr.m_context);
			if (newTexture->IsValid())
			{
				ID = (TextureID)mngr.m_textures.size();
				mngr.m_filepaths.emplace(filepath, ID);
				mngr.m_textures.emplace_back(newTexture.release());
			}
		}
		else
			ID = result->second;
	}

	return ID;
}

bool Graphics::DX11TextureManager::SetActive(TextureID ID)
{
	if (mngr.m_active == ID)
		return true;
	bool success = false;
	if (mngr.m_textures.size() > ID && ID >= 0)
	{
		if (mngr.m_textures[ID])
		{
			mngr.m_textures[ID]->SetAsActive();
			mngr.m_active = ID;
			success = true;
		}
	}
	else
	{
		SetDefaultActive(DefaultTextures::MODEL);
		mngr.m_active = -1;
		success = true;
	}
	return success;
}

void Graphics::DX11TextureManager::Clear()
{
	mngr.m_textures.clear();
	mngr.m_filepaths.clear();
}

std::string Graphics::DX11TextureManager::GetFilePath(TextureID id)
{
	std::string result;
	if (mngr.m_filepaths.size() > id && id >= 0)
	{
		std::wstring original;
		auto texture = mngr.m_filepaths.begin();
		std::advance(texture, id);
		if (texture != mngr.m_filepaths.end())
		{
			original = texture->first;
			result = std::string(original.begin(), original.end());
		}
	}

	return result;
}

void Graphics::DX11TextureManager::Initialize(ID3D11Device * device, ID3D11DeviceContext * context)
{
	mngr.m_device = device;
	mngr.m_context = context;
	mngr.m_defaultModelTexture = std::make_unique<DX11Texture>(L"EngineAssets/textures/white.dds", device, context);
	mngr.m_defaultTextTexture = std::make_unique<DX11Texture>(L"EngineAssets/textures/SpriteSheetx200.dds", device, context);
}

void Graphics::DX11TextureManager::SetDefaultActive(DefaultTextures type)
{
	switch (type)
	{
	case DefaultTextures::MODEL:
		mngr.m_defaultModelTexture->SetAsActive();
		break;
	case DefaultTextures::TEXT:
		mngr.m_defaultTextTexture->SetAsActive();
		break;
	default:
		break;
	}
}

std::wstring Graphics::DX11TextureManager::MultiByteToWideChar(std::string str)
{
	// room for 100 characters
	std::vector<WCHAR> buffer(100);
	::MultiByteToWideChar(
		CP_ACP,               // code page
		MB_PRECOMPOSED,       // character-type options
		str.c_str(),          // string to map
		(int)str.length(),         // number of bytes in string
		&buffer[0],           // wide-character buffer (must use vector here!)
		100                   // indexCount of buffer
	);
	return std::wstring(buffer.begin(), buffer.end());
}
