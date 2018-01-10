#pragma once
#include "WindowsAdditionals.h"


namespace Graphics
{
	struct DisplaySize
	{
		__engine_decl DisplaySize();
		__engine_decl DisplaySize(float width, float height);
		__engine_decl bool operator!=(const DisplaySize& other);
		float Width;
		float Height;
	};

	// Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
	interface IDeviceNotify
	{
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;
	};

	// Controls all the DirectX device resources.
	class DeviceResources
	{
	public:
		DeviceResources();
		void SetLogicalSize(DisplaySize logicalSize);
		void SetDpi(float dpi, DisplaySize size);
		void ValidateDevice();
		void HandleDeviceLost();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
		void SetHandleInfo(HWND hwnd, DisplaySize size);
		void Trim();
		void Present();
		void ClearScreen(float r, float g, float b);

		// The size of the render target, in pixels.
		DisplaySize	GetOutputSize() const { return m_outputSize; }

		// The size of the render target, in dips.
		DisplaySize	GetLogicalSize() const { return m_logicalSize; }
		float						GetDpi() const { return m_effectiveDpi; }

		// D3D Accessors.
		ID3D11Device3*		                           GetD3DDevice() const { return m_d3dDevice.Get(); }
		ID3D11DeviceContext3*                          GetD3DDeviceContext() const { return m_d3dContext.Get(); }
		IDXGISwapChain3*			                   GetSwapChain() const { return m_swapChain.Get(); }
		D3D_FEATURE_LEVEL			                   GetDeviceFeatureLevel() const { return m_d3dFeatureLevel; }
		ID3D11RenderTargetView*	                       GetBackBufferRenderTargetView() const { return m_d3dRenderTargetView.Get(); }
		ID3D11DepthStencilView*		                   GetDepthStencilView() const { return m_d3dDepthStencilView.Get(); }
		D3D11_VIEWPORT				                   GetScreenViewport() const { return m_screenViewport; }
									                   
		// D2D Accessors.			                   
		ID2D1Factory3*				                   GetD2DFactory() const { return m_d2dFactory.Get(); }
		ID2D1Device2*				                   GetD2DDevice() const { return m_d2dDevice.Get(); }
		ID2D1DeviceContext2*		                   GetD2DDeviceContext() const { return m_d2dContext.Get(); }
		ID2D1Bitmap1*				                   GetD2DTargetBitmap() const { return m_d2dTargetBitmap.Get(); }
		IDWriteFactory3*			                   GetDWriteFactory() const { return m_dwriteFactory.Get(); }
		IWICImagingFactory2*		                   GetWicImagingFactory() const { return m_wicFactory.Get(); }

	private:
		void CreateDeviceIndependentResource();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void UpdateRenderTargetSize();

		// Direct3D objects.
		Microsoft::WRL::ComPtr<ID3D11Device3>			m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext3>	m_d3dContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain3>			m_swapChain;

		// Direct3D rendering objects. Required for 3D.
		Microsoft::WRL::ComPtr<ID3D11Texture2D1>        m_backBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_d3dRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_d3dDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_offScreenSurface;
		D3D11_VIEWPORT									m_screenViewport;

		// Direct2D drawing components.
		Microsoft::WRL::ComPtr<ID2D1Factory3>		    m_d2dFactory;
		Microsoft::WRL::ComPtr<ID2D1Device2>		    m_d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext2>	    m_d2dContext;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>		    m_d2dTargetBitmap;
													    
		// DirectWrite drawing components.			    
		Microsoft::WRL::ComPtr<IDWriteFactory3>		    m_dwriteFactory;
		Microsoft::WRL::ComPtr<IWICImagingFactory2>	    m_wicFactory;

		// Cached device properties.
		D3D_FEATURE_LEVEL								m_d3dFeatureLevel;
		DisplaySize						                m_d3dRenderTargetSize;
		DisplaySize						                m_outputSize;
		DisplaySize						                m_logicalSize;
		float											m_dpi;

		//Main handle
		HWND m_hwnd;

		// This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
		float m_effectiveDpi;

		// The IDeviceNotify can be held directly as it owns the DeviceResources.
		IDeviceNotify* m_deviceNotify;
	};

}
