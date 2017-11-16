#pragma once
#include "DX11Shader.h"


namespace Graphics
{
	class DX11ShaderManager
	{
	public:
		DX11ShaderManager(ID3D11Device* device, ID3D11DeviceContext* context);
		~DX11ShaderManager();

		void CreateShader(std::string uniqueName, std::wstring vsPath, std::wstring psPath,
			std::wstring gsPath = L"", std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout = {});
		void DeleteShader(std::string uniqueName);
		bool SetActive(std::string uniqueName);
	private:

		std::string m_active;
		std::map<std::string, std::unique_ptr<DX11Shader>> m_shaders;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
	};
}


