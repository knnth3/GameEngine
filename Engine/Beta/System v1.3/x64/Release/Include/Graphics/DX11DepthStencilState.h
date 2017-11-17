#pragma once
#include <d3d11.h>
#include "DllSettings.h"

namespace Graphics
{
	class DX11DepthStencilState
	{
	public:
		DX11DepthStencilState(uint16_t windowWidth, uint16_t windowHeight, ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv);
		bool Initialize();
		void OnWindowResize(ID3D11RenderTargetView* rtv, uint16_t windowWidth, uint16_t windowHeight);
		void SetAsActive();
		void ClearView();
		void Close();
		~DX11DepthStencilState();

		//Change Z buffer status
		void SetDepthBufferStatus(const bool value);
	private:

		bool m_bBufferStatus;
		uint16_t m_windowWidth;
		uint16_t m_windowHeight;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		ID3D11RenderTargetView* m_rtv;
		D3D11_TEXTURE2D_DESC m_depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC m_3DdepthStencilDesc;
		D3D11_DEPTH_STENCIL_DESC m_2DdepthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC m_depthStencilViewDesc;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11DepthStencilState* m_3DdepthStencilState;
		ID3D11DepthStencilState* m_2DdepthStencilState;

	};
}

