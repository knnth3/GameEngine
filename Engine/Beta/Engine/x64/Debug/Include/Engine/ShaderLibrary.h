#pragma once
#include "Library.h"
#include "GraphicsShader.h"
#include "GraphicsBuffer.h"

namespace Graphics
{
	class ShaderLibrary
	{
	public:
		ShaderLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		bool Initialize(const std::string& defaultVsPath, const std::string& defaultPsPath);
		const std::shared_ptr<GraphicsShader> GetShader(const std::string& uniqueName);
		const std::shared_ptr<GraphicsShader> CreateShader(const std::string& uniqueName);
		void DeleteShader(const std::string& uniqueName);
		bool SetAsActive(const std::string& uniqueName);
		void Clear();

	private:

		std::string m_activeShader;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		std::map<std::string, int> m_shaderCodex;
		Library<GraphicsShader> m_shaders;

	};
}

