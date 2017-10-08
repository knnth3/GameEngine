#pragma once
#include <d3d11.h>
#include "D3Dcompiler.h"
#include "Lime.h"

namespace Lime
{
	class DX11Shader
	{
	public:
		//Default layout is vec3, vec2, vec3
		DX11Shader(const LPCWSTR vsPath, const LPCWSTR psPath, ID3D11Device* device, ID3D11DeviceContext* context);
		HRESULT Initialize();
		void Close();
		void SetAsActive();
		~DX11Shader();

		//Change Values
		void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout);

	private:
		HRESULT CompileShader(LPCWSTR srcFile, LPCSTR entryPoint,
			LPCSTR profile, ID3DBlob** blob);

		LPCWSTR m_vsPath;
		LPCWSTR m_psPath;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_layout;

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		ID3D11InputLayout* m_vertLayout;
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
	};
}

