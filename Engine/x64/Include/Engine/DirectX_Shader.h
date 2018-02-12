#pragma once
#include "WindowsAdditionals.h"

#define COMPILED_SHADER_EXT ".cso"
#define RAW_SHADER_EXT ".hlsl"

namespace Engine
{
	//Handles the creation and usage of all shader structures
	//Any given shader files must have the entry point as "main"
	//Version: hlsl 5.0
	class DirectX_Shader
	{
	public:
		DirectX_Shader(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		bool SetAsActive();

		//Must coincide with the default vertex layout.
		//Include the StandardDataStructures.hlsl file to get current Vertex structure.
		bool SetVertexShader(const std::string& vsPath, std::string* error = nullptr);
		bool SetPixelShader(const std::string& psPath, std::string* error = nullptr);
		bool SetGeometryShader(const std::string& gsPath, std::string* error = nullptr);

		bool HasVertexShader()const;
		bool HasPixelShader()const;
		bool HasGeometryShader()const;

		std::string GetVertexShaderPath()const;
		std::string GetPixelShaderPath()const;
		std::string GetGeometryShaderPath()const;
	private:
		void ClearVertexShader();
		void ClearPixelShader();
		void ClearGeometryShader();
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDescFromVertexLayout(std::string& retError);
		HRESULT CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, Microsoft::WRL::ComPtr<ID3DBlob>& blob, std::string* error = nullptr);
		int ReadBytes(const std::string& name, std::vector<byte>& bytes);
		bool IsCompiled(const std::string& filepath);

		std::string m_vsPath;
		std::string m_psPath;
		std::string m_gsPath;

		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>    m_vertexLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>   m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>    m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	};

}
