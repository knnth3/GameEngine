#pragma once
#include "Library.h"
#include "DirectX_Texture.h"

namespace Engine
{
	typedef int32_t TextureID;

	class TextureLibrary
	{
	public:
		__engine_decl TextureLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		__engine_decl bool Initialize(const std::string diffuse, const std::string normal, 
			const std::string emissive, const std::string roughness, const std::string metallic);
		__engine_decl bool CreateTexture(const std::string uniqueName, const std::string diffuse, 
			const std::string normal, const std::string emissive, const std::string roughness, const std::string metallic);
		__engine_decl void DeleteTexture(const std::string& uniqueName);
		__engine_decl bool SetAsActive(const std::string& uniqueName);
		__engine_decl void Clear();

	private:
		void CreateSamplerSate();

		std::string m_activeTexture;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		Library<DirectX_Texture> m_textures;
		std::map<std::string, int> m_textureCodex;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	};
}

