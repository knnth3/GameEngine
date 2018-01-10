#pragma once
#include "WindowsAdditionals.h"

namespace Graphics
{
	class GraphicsTexture
	{
	public:
		GraphicsTexture(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		bool Initialize(const std::string& diffuse, const std::string& normal, const std::string& emissive,
			const std::string& roughness, const std::string& metalic);
		void SetAsActive();

	private:
		bool LoadTexture(LPCWSTR filepath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& target);

		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		std::wstring m_tmFileNames[5];
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textures[5];
	};

}
