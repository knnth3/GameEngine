#pragma once
#include "DX11Shader.h"
#include <wrl.h>


namespace Graphics
{
	class DX11ShaderManager
	{
	public:
		DX11ShaderManager(ID3D11Device3* device, ID3D11DeviceContext3* context);
		~DX11ShaderManager();

		void CreateShader(std::string uniqueName, std::wstring path, std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout = {}, const bool bHasGS = false);
		void DeleteShader(std::string uniqueName);
		bool SetActive(std::string uniqueName);
	private:
		bool CreateBlendState();

		std::string m_active;
		std::map<std::string, std::unique_ptr<DX11Shader>> m_shaders;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;
	};
}


