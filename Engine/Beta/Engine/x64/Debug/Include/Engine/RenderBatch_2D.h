#pragma once
#include <wincodec.h>
#include "Text.h"
#include "Shape_Square.h"
#include "Line.h"

#ifdef LoadImage
#undef LoadImage
#endif

namespace Engine
{

	class RenderBatch_2D
	{
	public:
		__engine_decl RenderBatch_2D(IDWriteFactory3* writeFactory, 
			ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D, IWICImagingFactory2* wicFactory);
		__engine_decl ~RenderBatch_2D();
		__engine_decl void Initialize();
		__engine_decl void CreateDeviceDependentResources();
		__engine_decl void ReleaseDeviceDependentResources();
		__engine_decl void SetDimensions(float width, float height);
		__engine_decl void BeginScene();
		__engine_decl void ClearScreen(float r, float g, float b, float a = 1.0f);
		__engine_decl void EndScene();
		__engine_decl void Draw(const Text& t);
		__engine_decl void Draw(const Square& s, bool background = false);
		__engine_decl void Draw(const Line& l);

	private:
		void CreateEffects();
		void DrawBitmap(const Square & s, bool background);
		void DrawBitmapBrush(const Square& s, bool background);
		void DrawRect(const Square & s, bool background, bool defaultBrush = false);

		// Cached pointer to device resources.
		IDWriteFactory3* m_writeFactory;
		ID2D1Factory3* m_2DFactory;
		ID2D1DeviceContext* m_2DDeviceContext;
		IWICImagingFactory2* m_wicFactory;

		// Resources related to text rendering.
		float m_windowWidth;
		float m_windowHeight;
		Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> m_bitmapRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> m_backgroundRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> m_textRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_defualtBrush;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
	};

}