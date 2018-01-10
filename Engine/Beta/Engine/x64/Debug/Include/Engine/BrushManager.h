#pragma once
#include "WindowsAdditionals.h"
#include "Effect_2D.h"
#include <wincodec.h>

namespace Graphics
{
	enum BRUSH_TEXTURE_TYPE
	{
		BRUSH_TEXTURE_DEFAULT,
		BRUSH_TEXTURE_IMAGE_BRUSH,
		BRUSH_TEXTURE_IMAGE
	};

	class BrushManager
	{
	public:

		__engine_decl static bool Initialize(ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D, IWICImagingFactory2* wicFactory);
		__engine_decl static void Reset();

		__engine_decl static int CreateNewBrush(glm::vec4 color);
		__engine_decl static int CreateNewImageBrush(const int imageID, std::wstring& err);
		__engine_decl static int CreateNewImage(std::string filename, std::wstring& err);
		__engine_decl static void AddEffect(std::shared_ptr<Effect_2D> effect);

		__engine_decl static void DeleteBrush(const int ID);
		__engine_decl static void DeleteImageBrush(const int ID);
		__engine_decl static void DeleteImage(const int ID);
		__engine_decl static void ClearEffect();

		__engine_decl static const bool GetImageDimensions(const int ID, glm::vec2& dims);
		__engine_decl static const std::shared_ptr<Effect_2D> GetEffect();
		__engine_decl static const Microsoft::WRL::ComPtr<ID2D1Bitmap> GetImage(int ID);
		__engine_decl static const Microsoft::WRL::ComPtr<ID2D1BitmapBrush1> GetImageBrush(int ID);
		__engine_decl static const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> GetBrush(int ID);

	private:
		template<class T> static int FindNextEmpty(std::vector<T>& arr);
		template<class T> static void SafeClose(std::vector<Microsoft::WRL::ComPtr<T>>& arr);
		template<class T> static void SafeClose(std::vector<Microsoft::WRL::ComPtr<T>>& arr, int ID);
		static bool LoadImageFromFile(std::wstring filename, Microsoft::WRL::ComPtr<ID2D1Bitmap>& bitmap, std::wstring& str);

		static std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap>> m_images;
		static std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> m_solidBrushes;
		static std::vector<Microsoft::WRL::ComPtr<ID2D1BitmapBrush1>> m_imageBrushes;
		static std::shared_ptr<Effect_2D> m_effect;
		static ID2D1Factory3* m_2DFactory;
		static ID2D1DeviceContext* m_2DDeviceContext;
		static IWICImagingFactory2* m_wicFactory;
	};


}