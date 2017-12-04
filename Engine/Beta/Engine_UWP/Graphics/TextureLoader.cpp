#include "TextureLoader.h"
static Graphics::TextureLoader mngr;


Graphics::TextureLoader::TextureLoader()
{
	m_active = -1;
	m_device = nullptr;
	m_context = nullptr;
	m_defaultModelTexture = nullptr;
}

Graphics::TextureID Graphics::TextureLoader::CreateNewTexture(const std::string uniqueName, const std::string diffuse, 
	const std::string normal, const std::string emissive, const std::string roughness, const std::string metalic)
{
	TextureID ID = -1;
	auto name = mngr.MultiByteToWideChar(uniqueName);
	if (mngr.m_device)
	{
		auto result = mngr.m_uniqueNames.find(name);
		if (result == mngr.m_uniqueNames.end())
		{
			std::vector<std::wstring> paths;
			paths.push_back(mngr.MultiByteToWideChar(diffuse));
			paths.push_back(mngr.MultiByteToWideChar(normal));
			paths.push_back(mngr.MultiByteToWideChar(emissive));
			paths.push_back(mngr.MultiByteToWideChar(roughness));
			paths.push_back(mngr.MultiByteToWideChar(metalic));

			auto newTexture = std::make_unique<DX11Texture>(paths[0].c_str(), paths[1].c_str(), paths[2].c_str(), 
				paths[3].c_str(), paths[4].c_str(), mngr.m_device, mngr.m_context);
			if (newTexture->Initialize())
			{
				ID = (TextureID)mngr.m_textures.size();
				mngr.m_uniqueNames.emplace(name, ID);
				mngr.m_textures.emplace_back(newTexture.release());
			}
		}
		else
			ID = result->second;
	}

	return ID;
}

bool Graphics::TextureLoader::SetActive(TextureID ID)
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
		SetDefaultActive();
		mngr.m_active = -1;
		success = true;
	}
	return success;
}

void Graphics::TextureLoader::Clear()
{
	mngr.m_textures.clear();
	mngr.m_uniqueNames.clear();
}

bool Graphics::TextureLoader::Initialize(ID3D11Device3 * device, ID3D11DeviceContext3 * context)
{
	bool result = false;
	mngr.m_device = device;
	mngr.m_context = context;
	std::wstring diffuse =   L"Assets/textures/Default/diffuse.png";
	std::wstring normal =    L"Assets/textures/Default/normal.png";
	std::wstring emissive =  L"Assets/textures/Default/emissive.png";
	std::wstring roughness = L"Assets/textures/Default/roughness.png";
	std::wstring metallic =  L"Assets/textures/Default/metallic.png";
	mngr.m_defaultModelTexture = std::make_unique<DX11Texture>(diffuse.c_str(), normal.c_str(), emissive.c_str(),
		roughness.c_str(), metallic.c_str(), mngr.m_device, mngr.m_context);
	result = mngr.m_defaultModelTexture->Initialize();
	if(result)
		mngr.m_defaultModelTexture->SetAsActive();
	return result;
}

void Graphics::TextureLoader::SetDefaultActive()
{
	mngr.m_defaultModelTexture->SetAsActive();
}

std::wstring Graphics::TextureLoader::MultiByteToWideChar(std::string str)
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
