#include "TextureManager.h"
static Lime::TextureManager mngr;

Lime::TextureID Lime::TextureManager::CreateNewTexture(const LPCWSTR filepath)
{
	TextureID ID = -1;
	try
	{
		auto newTexture = std::make_unique<DX11Texture>(filepath, mngr.m_device, mngr.m_context);
		mngr.m_textures.emplace_back(nullptr);
		ID = (TextureID)mngr.m_textures.size() - 1;
		newTexture.swap(mngr.m_textures[(size_t)ID]);
	}
	catch (const std::exception&)
	{
		//TODO: Do something
	}

	return ID;
}

void Lime::TextureManager::SetActive(TextureID ID)
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

void Lime::TextureManager::Clear()
{
	mngr.m_textures.clear();
}

void Lime::TextureManager::Initialize(ID3D11Device * device, ID3D11DeviceContext * context)
{
	mngr.m_device = device;
	mngr.m_context = context;
	mngr.m_defaultModelTexture = std::make_unique<DX11Texture>(L"EngineAssets/textures/white.dds", device, context);
	mngr.m_defaultTextTexture = std::make_unique<DX11Texture>(L"EngineAssets/textures/SpriteSheetx200.dds", device, context);
}

void Lime::TextureManager::SetDefaultActive(DefaultTextures type)
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