#pragma once
#include <D3D11_3.h>
#include "WindowsAdditionals.h"

namespace Graphics
{
	class DX11Texture
	{
	public:
		DX11Texture(const LPCWSTR diffuse, const LPCWSTR normal, const LPCWSTR emissive, 
			const LPCWSTR roughness, const LPCWSTR metalic, ID3D11Device3* device, ID3D11DeviceContext3* context);
		bool Initialize();
		void SetAsActive();
		~DX11Texture();

	private:
		bool LoadTexture(LPCWSTR filepath, ID3D11ShaderResourceView*& target);
		void CreateSamplerSate();

		LPCWSTR m_diffuseMap;
		LPCWSTR m_normalMap;
		LPCWSTR m_emissiveMap;
		LPCWSTR m_roughnessMap;
		LPCWSTR m_metalicMap;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		LPCWSTR m_tmFileNames[5];
		ID3D11ShaderResourceView* m_textures[5];
	};

}
