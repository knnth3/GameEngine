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
		__engine_decl Text();
		__engine_decl Text(std::wstring str, std::string style);
		__engine_decl Text(std::string str, std::string style);

		//Set
		__engine_decl void SetFormat(std::string style);
		__engine_decl void SetPosition(float x, float y);
		__engine_decl void SetPosition(glm::vec2 pos);
		__engine_decl void SetString(std::string val);
		__engine_decl void SetString(std::wstring val);
		__engine_decl void SetBounds(float width, float height);
		__engine_decl void SetBrush(int ID);

		//Get

		__engine_decl std::string GetFormatName()const;
		__engine_decl D2D_POINT_2F GetPosition()const;
		__engine_decl glm::vec2 GetBounds()const;
		__engine_decl int GetBrush()const;

		//Operator
		__engine_decl virtual void operator=(std::string str);
		__engine_decl virtual void operator=(std::wstring str);

	private:

		D2D_POINT_2F m_position;
		glm::vec2 m_bounds;
		std::string m_style;
		int m_brush;
	};

	class TextStyleLib
	{
	public:
		__engine_decl static bool Initialize(IDWriteFactory3* writeFactory);
		__engine_decl static void Close();
		__engine_decl static bool CreateNewFormat(std::string uniqueName, TextFormat format);
		__engine_decl static void ClearFormats();
		__engine_decl static const Microsoft::WRL::ComPtr<IDWriteTextFormat2> GetFormat(std::string uniqueName);

	private:

		static IDWriteFactory3* m_factory;
		static std::map<std::string, Microsoft::WRL::ComPtr<IDWriteTextFormat2>> m_formats;
	};

}