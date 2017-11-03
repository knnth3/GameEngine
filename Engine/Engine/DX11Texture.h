#pragma once
#include <d3d11.h>
#include "Shared.h"

namespace Lime
{
	class DX11Texture
	{
	public:
		DX11Texture(const LPCWSTR filepath, ID3D11Device* device, ID3D11DeviceContext* context);
		bool IsValid();
		void SetAsActive();
		~DX11Texture();

	private:
		void Initialize();
		void CreateSamplerSate();

		bool m_bIsValid;
		LPCWSTR m_filepath;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		ID3D11ShaderResourceView* m_texture;
	};

}
