#pragma once
#include "WindowsAdditionals.h"

namespace Graphics
{
	struct TextStyle
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
		GRAPHICS_DLL_API void SetStyle(std::string style);
		GRAPHICS_DLL_API void SetPosition(float x, float y);
		GRAPHICS_DLL_API void SetPosition(glm::vec2 pos);

		//Get
		GRAPHICS_DLL_API std::string GetStyleName()const;
		GRAPHICS_DLL_API glm::vec2 GetPosition()const;
		GRAPHICS_DLL_API D2D1::Matrix3x2F GetTranslation()const;

		//Operator
		GRAPHICS_DLL_API void operator=(std::string& str);
		GRAPHICS_DLL_API void operator=(std::wstring& str);

	private:
		std::string m_style;
		glm::vec2 m_position;
		D2D1::Matrix3x2F m_translation;
	};

}