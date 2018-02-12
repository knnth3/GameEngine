#pragma once
#include "DX11Texture.h"
#include "DllSettings.h"
#include <unordered_map>

namespace Graphics
{
	typedef int32_t TextureID;

	class TextureLoader
	{
	public:
		GRAPHICS_DLL_API TextureLoader();
		GRAPHICS_DLL_API static TextureID CreateNewTexture(const std::string uniqueName, const std::string diffuse, 
			const std::string normal, const std::string emissive, const std::string roughness, const std::string metalic);
		GRAPHICS_DLL_API static bool SetActive(TextureID ID);
		GRAPHICS_DLL_API static void Clear();

		//Sets default Textures for fallback
		GRAPHICS_DLL_API static bool Initialize(ID3D11Device3* device, ID3D11DeviceContext3* context);
		GRAPHICS_DLL_API static void SetDefaultActive();

	private:
		std::wstring MultiByteToWideChar(std::string str);

		TextureID m_active;
		std::unordered_map<std::wstring, TextureID> m_uniqueNames;
		std::unique_ptr<DX11Texture> m_defaultModelTexture;
		std::vector<std::unique_ptr<DX11Texture>> m_textures;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;

	};
}

