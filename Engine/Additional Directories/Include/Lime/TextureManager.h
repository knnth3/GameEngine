#pragma once
#include "DX11Texture.h"


namespace Lime
{
	typedef int32_t TextureID;

	enum class DefaultTextures
	{
		MODEL,
		TEXT
	};

	class TextureManager
	{
		friend class DX11Graphics;
	public:

		AppDLL_API static TextureID CreateNewTexture(const LPCWSTR filepath);
		AppDLL_API static void SetActive(TextureID ID);
		AppDLL_API static void Clear();

	protected:
		//Sets default Textures for fallback
		AppDLL_API static void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
		AppDLL_API static void SetDefaultActive(DefaultTextures type);

	private:
		std::unique_ptr<DX11Texture> m_defaultModelTexture;
		std::unique_ptr<DX11Texture> m_defaultTextTexture;
		std::vector<std::unique_ptr<DX11Texture>> m_textures;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;

	};
}

