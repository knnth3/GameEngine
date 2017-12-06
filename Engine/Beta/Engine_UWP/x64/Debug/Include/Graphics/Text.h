#pragma once
#include "WindowsAdditionals.h"

namespace Graphics
{
	struct TextFormat
	{
		std::wstring FontFamilyName;
		float FontSize;
	};

	class Text:
		public std::wstring
	{
	public:
		GRAPHICS_DLL_API Text();
		GRAPHICS_DLL_API Text(std::wstring str, std::string style);
		GRAPHICS_DLL_API Text(std::string str, std::string style);

		//Set
		GRAPHICS_DLL_API void SetFormat(std::string style);
		GRAPHICS_DLL_API void SetPosition(float x, float y);
		GRAPHICS_DLL_API void SetPosition(glm::vec2 pos);
		GRAPHICS_DLL_API void SetString(std::string val);
		GRAPHICS_DLL_API void SetString(std::wstring val);
		GRAPHICS_DLL_API void SetBounds(float width, float height);
		GRAPHICS_DLL_API void SetBrush(std::string uniqueName);

		//Get

		GRAPHICS_DLL_API std::string GetFormatName()const;
		GRAPHICS_DLL_API D2D_POINT_2F GetPosition()const;
		GRAPHICS_DLL_API glm::vec2 GetBounds()const;
		GRAPHICS_DLL_API std::string GetBrushName()const;

		//Operator
		GRAPHICS_DLL_API virtual void operator=(std::string str);
		GRAPHICS_DLL_API virtual void operator=(std::wstring str);

	private:

		D2D_POINT_2F m_position;
		glm::vec2 m_bounds;
		std::string m_style;
		std::string m_brush;
	};

	class TextStyleLib
	{
	public:
		GRAPHICS_DLL_API static bool Initialize(IDWriteFactory3* writeFactory);
		GRAPHICS_DLL_API static void Close();
		GRAPHICS_DLL_API static bool CreateNewFormat(std::string uniqueName, TextFormat format);
		GRAPHICS_DLL_API static void ClearFormats();
		GRAPHICS_DLL_API static const Microsoft::WRL::ComPtr<IDWriteTextFormat2> GetFormat(std::string uniqueName);

	private:

		static IDWriteFactory3* m_factory;
		static std::map<std::string, Microsoft::WRL::ComPtr<IDWriteTextFormat2>> m_formats;
	};

}