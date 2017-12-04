#pragma once
#include "Text.h"

namespace Graphics
{
	class RenderBatch_2D
	{
	public:
		GRAPHICS_DLL_API RenderBatch_2D(IDWriteFactory3* writeFactory, ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D);
		GRAPHICS_DLL_API void Initialize();
		GRAPHICS_DLL_API void CreateDeviceDependentResources();
		GRAPHICS_DLL_API void ReleaseDeviceDependentResources();
		GRAPHICS_DLL_API void SetDimensions(float width, float height);
		GRAPHICS_DLL_API void BeginScene();
		GRAPHICS_DLL_API void EndScene();
		GRAPHICS_DLL_API void Draw(const Text& t);
		GRAPHICS_DLL_API void Render();

	private:
		// Cached pointer to device resources.
		IDWriteFactory3* m_writeFactory;
		ID2D1Factory3* m_2DFactory;
		ID2D1DeviceContext* m_2DDeviceContext;

		// Resources related to text rendering.
		float m_windowWidth;
		float m_windowHeight;
		std::wstring                                    m_text;
		DWRITE_TEXT_METRICS	                            m_textMetrics;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_whiteBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_blackBrush;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
		Microsoft::WRL::ComPtr<IDWriteTextLayout3>      m_textLayout;
		Microsoft::WRL::ComPtr<IDWriteTextFormat2>      m_textFormat;
	};

}