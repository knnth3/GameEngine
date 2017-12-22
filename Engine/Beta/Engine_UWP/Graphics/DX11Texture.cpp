#include "DX11Texture.h"
#include <DirectXTex\DirectXTex.h>

static ID3D11SamplerState* m_samplerState = nullptr;

using namespace DirectX;

Graphics::DX11Texture::DX11Texture(const LPCWSTR diffuse, const LPCWSTR normal, const LPCWSTR emissive,
	const LPCWSTR roughness, const LPCWSTR metalic, ID3D11Device3* device, ID3D11DeviceContext3* context)
{
	m_samplerState = nullptr;
	m_tmFileNames[0] = diffuse;
	m_tmFileNames[1] = normal;
	m_tmFileNames[2] = emissive;
	m_tmFileNames[3] = roughness;
	m_tmFileNames[4] = metalic;
	m_device = device;
	m_context = context;
	if (!m_samplerState)
		CreateSamplerSate();
}

bool Graphics::DX11Texture::Initialize()
{
	//Load all texure maps
	for (int x = 0; x < 5; x++)
	{
		if (!LoadTexture(m_tmFileNames[x], m_textures[x]))
			return false;
	}
	return true;
}

void Graphics::DX11Texture::SetAsActive()
{
	if (!m_samplerState)
		CreateSamplerSate();
	m_context->PSSetShaderResources(0, 5, m_textures);
}

Graphics::DX11Texture::~DX11Texture()
{
	for(int x = 0; x < 5; x++)
		CLOSE_COM_PTR(m_textures[x]);

	CLOSE_COM_PTR(m_samplerState);
}

bool Graphics::DX11Texture::LoadTexture(LPCWSTR filepath, ID3D11ShaderResourceView*& target)
{
	HRESULT hr;
	bool result = false;
	wchar_t ext[_MAX_EXT];
	wchar_t fname[_MAX_FNAME];
	errno_t err = _wsplitpath_s(filepath, nullptr, 0, nullptr, 0, fname, _MAX_FNAME, ext, _MAX_EXT);
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
	}
	return result;
}

void Graphics::DX11Texture::CreateSamplerSate()
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
	m_context->PSSetSamplers(0, 1, &m_samplerState);
}
