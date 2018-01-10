#pragma once
#include "Library.h"
#include "GraphicsTexture.h"

namespace Graphics
{
	typedef int32_t TextureID;

	class TextureLibrary
	{
	public:
		GRAPHICS_DLL_API TextureLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		GRAPHICS_DLL_API bool Initialize(const std::string diffuse, const std::string normal, 
			const std::string emissive, const std::string roughness, const std::string metallic);
		GRAPHICS_DLL_API bool CreateTexture(const std::string uniqueName, const std::string diffuse, 
			const std::string normal, const std::string emissive, const std::string roughness, const std::string metallic);
		GRAPHICS_DLL_API void DeleteTexture(const std::string& uniqueName);
		GRAPHICS_DLL_API bool SetAsActive(const std::string& uniqueName);
		GRAPHICS_DLL_API void Clear();

	private:
		void CreateSamplerSate();

		std::string m_activeTexture;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		Library<GraphicsTexture> m_textures;
		std::map<std::string, int> m_textureCodex;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	};
}

