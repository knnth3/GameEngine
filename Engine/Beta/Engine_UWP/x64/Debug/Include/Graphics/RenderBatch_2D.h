#pragma once
#include <wincodec.h>
#include "Text.h"
#include "Shape_Square.h"
#include "Line.h"

#ifdef LoadImage
#undef LoadImage
#endif

namespace Graphics
{

	class RenderBatch_2D
	{
	public:
		GRAPHICS_DLL_API RenderBatch_2D(IDWriteFactory3* writeFactory, 
			ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D, IWICImagingFactory2* wicFactory);
		GRAPHICS_DLL_API ~RenderBatch_2D();
		GRAPHICS_DLL_API void Initialize();
		GRAPHICS_DLL_API void CreateDeviceDependentResources();
		GRAPHICS_DLL_API void ReleaseDeviceDependentResources();
		GRAPHICS_DLL_API void SetDimensions(float width, float height);
		GRAPHICS_DLL_API void BeginScene();
		GRAPHICS_DLL_API void EndScene();
		GRAPHICS_DLL_API void Draw(const Text& t);
		GRAPHICS_DLL_API void Draw(const Square& s);
		GRAPHICS_DLL_API void Draw(const Line& l);
		GRAPHICS_DLL_API void CreateNewBrush(std::string uniqueName, glm::vec4 color, bool newColor = true);
		GRAPHICS_DLL_API void DeleteBrush(std::string uniqueName);
		GRAPHICS_DLL_API void DeleteImageBrush(std::string uniqueName);
		GRAPHICS_DLL_API void CreateNewImageBrush(std::string uniqueName, std::string filenam, bool newBrush = true);

	private:
		HRESULT LoadResourceBitmapToBrush(std::wstring filename, Microsoft::WRL::ComPtr<ID2D1Bitmap>& bitmap);
		Microsoft::WRL::ComPtr<ID2D1Bitmap> GetBitmap(std::string uniqueName);
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> GetBrush(std::string uniqueName);

		// Cached pointer to device resources.
		IDWriteFactory3* m_writeFactory;
		ID2D1Factory3* m_2DFactory;
		ID2D1DeviceContext* m_2DDeviceContext;
		IWICImagingFactory2* m_wicFactory;

		// Resources related to text rendering.
		float m_windowWidth;
		float m_windowHeight;
		std::map<std::string, glm::vec4> m_brushColors;
		std::map<std::string, std::string> m_bmpBrushInfo;
		std::map<std::string, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> m_solidBrushes;
		std::map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap>> m_bitmaps;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_defualtBrush;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
		Microsoft::WRL::ComPtr<IWICFormatConverter> m_formatConvert;
	};

}