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
		GRAPHICS_DLL_API DX11Shader(const LPCWSTR path, ID3D11Device* device, ID3D11DeviceContext* context, const bool bHasGS);
		GRAPHICS_DLL_API HRESULT Initialize();
		GRAPHICS_DLL_API void Close();
		GRAPHICS_DLL_API void SetAsActive();
		GRAPHICS_DLL_API ~DX11Shader();

		//Change Values
		GRAPHICS_DLL_API void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& newLayout);

	private:
		HRESULT CompileShader(LPCWSTR srcFile, LPCSTR entryPoint,
			LPCSTR profile, ID3DBlob** blob);

		bool m_hasGS;
		LPCWSTR m_filePath;

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

