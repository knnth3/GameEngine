#include "DirectX_Texture.h"
#include <DirectXTex\DirectXTex.h>


using namespace DirectX;
using namespace Microsoft::WRL;

Engine::DirectX_Texture::DirectX_Texture(ID3D11Device3 * device, ID3D11DeviceContext3 * context)
{
	m_device = device;
	m_context = context;
}

bool Engine::DirectX_Texture::Initialize(const std::string& diffuse, const std::string& normal, const std::string& emissive,
	const std::string& roughness, const std::string& metalic)
{
	m_tmFileNames[0] = To_wstr(diffuse);
	m_tmFileNames[1] = To_wstr(normal);
	m_tmFileNames[2] = To_wstr(emissive);
	m_tmFileNames[3] = To_wstr(roughness);
	m_tmFileNames[4] = To_wstr(metalic);
	//Load all texure maps
	for (int x = 0; x < 5; x++)
	{
		if (!LoadTexture(m_tmFileNames[x].c_str(), m_textures[x]))
			return false;
	}
	return true;
}

void Engine::DirectX_Texture::SetAsActive()
{
	m_context->PSSetShaderResources(0, 5, m_textures->GetAddressOf());
}

bool Engine::DirectX_Texture::LoadTexture(LPCWSTR filepath, ComPtr<ID3D11ShaderResourceView>& target)
{
	HRESULT hr;
	bool result = false;
	wchar_t ext[_MAX_EXT];
	wchar_t fname[_MAX_FNAME];
	_wsplitpath_s(filepath, nullptr, 0, nullptr, 0, fname, _MAX_FNAME, ext, _MAX_EXT);
	ScratchImage srcImage;
	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = LoadFromDDSFile(filepath, DDS_FLAGS_NONE, nullptr, srcImage);
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
