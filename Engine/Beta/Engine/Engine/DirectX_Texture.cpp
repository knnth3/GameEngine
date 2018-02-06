#include "DirectX_Texture.h"
#include <DirectXTex\DirectXTex.h>


using namespace DirectX;
using namespace Microsoft::WRL;

Engine::DirectX_Texture::DirectX_Texture(ID3D11Device3 * device, ID3D11DeviceContext3 * context)
{
	m_device = device;
	m_context = context;
}

bool Engine::DirectX_Texture::Initialize(const std::string& m_path)
{
	m_tmFileNames.clear();
	m_textures.clear();
	m_tmFileNames.push_back(To_wstr(m_path));
	m_textures.resize(1, nullptr);
	if (!LoadTexture(m_tmFileNames[0].c_str(), m_textures[0]))
		return false;

	return true;
}

bool Engine::DirectX_Texture::Initialize(const std::vector<std::string>& paths)
{
	if (paths.size() > MAX_TEXTURE_BINDS)
	{
		std::wstring message = 
			std::wstring(L"Max texture binds: ") + 
			std::to_wstring(MAX_TEXTURE_BINDS) + 
			std::wstring(L"\nOwner: ") + 
			std::wstring(To_wstr(paths[0]));

		OpenDialog(L"Texture init failed!", message.c_str());
		return false;
	}

	m_tmFileNames.clear();
	m_textures.clear();
	for (auto& path : paths)
	{
		m_tmFileNames.push_back(To_wstr(path));
		m_textures.push_back(nullptr);
	}

	//Load all texure maps
	for (auto index = 0; index < m_textures.size(); index++)
	{
		if (!LoadTexture(m_tmFileNames[index].c_str(), m_textures[index]))
			return false;
	}
	return true;
}

void Engine::DirectX_Texture::SetAsActive()
{
	m_context->PSSetShaderResources(0, (UINT)m_textures.size(), m_textures.data()->GetAddressOf());
}

bool Engine::DirectX_Texture::LoadTexture(LPCWSTR filepath, ComPtr<ID3D11ShaderResourceView>& target)
{
	HRESULT hr;
	bool result = false;
	wchar_t ext[_MAX_EXT];
	wchar_t fname[_MAX_FNAME];
	_wsplitpath_s(filepath, nullptr, 0, nullptr, 0, fname, _MAX_FNAME, ext, _MAX_EXT);
	ScratchImage srcImage = {};
	TexMetadata info = {};
	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = LoadFromDDSFile(filepath, DDS_FLAGS_NONE, &info, srcImage);
		result = CheckSuccess(hr, filepath);
	}
	else
	{
		hr = LoadFromWICFile(filepath, WIC_FLAGS_FILTER_LINEAR, nullptr, srcImage);
		result = CheckSuccess(hr, filepath);
	}
	if (result)
	{
		ScratchImage secondary;

		hr = GenerateMipMaps(srcImage.GetImages(), srcImage.GetImageCount(),
			srcImage.GetMetadata(), TEX_FILTER_DEFAULT, 0, secondary);
		result = CheckSuccess(hr, filepath);

		hr = CreateShaderResourceView(m_device, secondary.GetImages(), secondary.GetImageCount(), secondary.GetMetadata(), &target);
		result = CheckSuccess(hr, filepath);
		secondary.Release();
	}
	srcImage.Release();
	return result;
}
