#pragma once
#include "WindowsAdditionals.h"


namespace Graphics
{
	class GraphicsRSS
	{
	public:
		GraphicsRSS(ID3D11Device3 * device, ID3D11DeviceContext3 * context, const std::string& name);
		bool Initialize(D3D11_RASTERIZER_DESC desc);
		void SetAsActive();
	private:
		
		std::string m_name;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
	};

}
