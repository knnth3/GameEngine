#include "TextureManager.h"
static Graphics::TextureManager mngr;

Graphics::TextureID Graphics::TextureManager::CreateNewTexture(const std::wstring filepath)
{
	TextureID ID = -1;
	if(!filepath.empty())
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

void Graphics::TextureManager::SetActive(TextureID ID)
{
	try
	{
		mngr.m_textures[ID]->SetAsActive();
	}
	catch (const std::exception&)
	{
		//TODO: Do something
	}
}

void Graphics::TextureManager::Clear()
{
	mngr.m_textures.clear();
	mngr.m_filepaths.clear();
}

std::string Graphics::TextureManager::GetFilePath(TextureID id)
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

void Graphics::TextureManager::Initialize(ID3D11Device * device, ID3D11DeviceContext * context)
{
	mngr.m_device = device;
	mngr.m_context = context;
	mngr.m_defaultModelTexture = std::make_unique<DX11Texture>(L"EngineAssets/textures/white.dds", device, context);
	mngr.m_defaultTextTexture = std::make_unique<DX11Texture>(L"EngineAssets/textures/SpriteSheetx200.dds", device, context);
}

void Graphics::TextureManager::SetDefaultActive(DefaultTextures type)
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