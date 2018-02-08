#pragma once
#include "Library.h"
#include "DirectX_Shader.h"

namespace Engine
{
	class ShaderLibrary
	{
	public:
		ShaderLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		bool Initialize(const std::string& defaultVsPath, const std::string& defaultPsPath, 
			std::string* error = nullptr, const std::string& fallbackVsPath = "", const std::string& fallbackPsPath = "");
		const std::shared_ptr<DirectX_Shader> GetShader(const std::string& uniqueName);
		const std::shared_ptr<DirectX_Shader> CreateShader(const std::string& uniqueName);
		void DeleteShader(const std::string& uniqueName);
		bool SetAsActive(const std::string& uniqueName);
		void Clear();

	private:

		std::string m_activeShader;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		std::map<std::string, int> m_shaderCodex;
		Library<DirectX_Shader> m_shaders;

	};
}

