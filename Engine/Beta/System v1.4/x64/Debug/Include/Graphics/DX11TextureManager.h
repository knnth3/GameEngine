#pragma once
#include "DX11Texture.h"
#include "DllSettings.h"
#include <unordered_map>

namespace Graphics
{
	typedef int32_t TextureID;

	class DX11TextureManager
	{
	public:
		GRAPHICS_DLL_API DX11TextureManager();
		GRAPHICS_DLL_API static TextureID CreateNewTexture(const std::string filepath);
		GRAPHICS_DLL_API static bool SetActive(TextureID ID);
		GRAPHICS_DLL_API static void Clear();
		GRAPHICS_DLL_API static std::string GetFilePath(TextureID id);

		//Sets default Textures for fallback
		GRAPHICS_DLL_API static void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
		GRAPHICS_DLL_API static void SetDefaultActive();

	private:
		std::wstring MultiByteToWideChar(std::string str);

		TextureID m_active;
		std::unordered_map<std::wstring, TextureID> m_filepaths;
		std::unique_ptr<DX11Texture> m_defaultModelTexture;
		std::vector<std::unique_ptr<DX11Texture>> m_textures;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;

	};
}

