#pragma once
#include <d3d11.h>

namespace Graphics
{
	class DxTexure
	{
	public:
		DxTexure(const LPCWSTR filepath, ID3D11Device* device, ID3D11DeviceContext* context);
		bool IsValid();
		void SetAsActive();
		~DxTexure();

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

