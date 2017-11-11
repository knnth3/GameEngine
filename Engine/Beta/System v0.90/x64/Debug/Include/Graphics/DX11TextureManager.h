#pragma once
#include "DX11Texture.h"
#include "DllSettings.h"
#include <unordered_map>

namespace Graphics
{
	typedef int32_t TextureID;

	enum class DefaultTextures
	{
		MODEL,
		TEXT
	};

	class DX11TextureManager
	{
	public:

		DLL_API static TextureID CreateNewTexture(const std::wstring filepath);
		DLL_API static void SetActive(TextureID ID);
		DLL_API static void Clear();
		DLL_API static std::string GetFilePath(TextureID id);

		//Sets default Textures for fallback
		DLL_API static void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
		DLL_API static void SetDefaultActive(DefaultTextures type);

	private:
		std::unordered_map<std::wstring, TextureID> m_filepaths;
		std::unique_ptr<DX11Texture> m_defaultModelTexture;
		std::unique_ptr<DX11Texture> m_defaultTextTexture;
		std::vector<std::unique_ptr<DX11Texture>> m_textures;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;

	};
}

