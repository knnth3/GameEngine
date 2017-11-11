#pragma once
#include <d3d11.h>
#include <map>
#include "D3Dcompiler.h"
#include "DllSettings.h"
#include "DX11ConstantBuffer.h"

namespace Graphics
{

	class DX11Shader
	{
		friend class DX11Graphics;
	public:
		//Default layout is vec3, vec2, vec3
		DLL_API DX11Shader(const LPCWSTR vsPath, const LPCWSTR psPath, ID3D11Device* device, ID3D11DeviceContext* context, const LPCWSTR gsPath = L"\0");
		DLL_API HRESULT Initialize();
		DLL_API void Close();
		DLL_API void SetAsActive();
		DLL_API ~DX11Shader();

		//Change Values
		void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout);

	protected:
		void AttachConstBufferManager(std::shared_ptr<DX11ConstantBuffer>& constbuff);

		void CreateConstantBuffer(const D3D11_BUFFER_DESC & desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA * data = nullptr);

		void SetConstBufferData(const std::string & uniqueName, void * data, const ShaderType type);

	private:
		HRESULT CompileShader(LPCWSTR srcFile, LPCSTR entryPoint,
			LPCSTR profile, ID3DBlob** blob);

		bool m_hasGS;
		LPCWSTR m_vsPath;
		LPCWSTR m_gsPath;
		LPCWSTR m_psPath;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_layout;
		std::shared_ptr<DX11ConstantBuffer> m_cbManager;

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		ID3D11InputLayout* m_vertLayout;
		ID3D11VertexShader* m_vertexShader;
		ID3D11GeometryShader* m_geometryShader;
		ID3D11PixelShader* m_pixelShader;
	};
}

