#pragma once
#include "WindowsAdditionals.h"

#define MAX_TEXTURE_BINDS 5

namespace Engine
{

	class DirectX_Texture
	{
	public:
		DirectX_Texture(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		bool Initialize(const std::string& m_path);
		bool Initialize(const std::vector<std::string>& paths);
		void SetAsActive();

	private:
		bool LoadTexture(LPCWSTR filepath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& target);

		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		std::vector<std::wstring> m_tmFileNames;
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;
	};

}
