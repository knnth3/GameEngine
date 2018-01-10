#pragma once
#include "Square.h"

namespace LIME_ENGINE
{
	class Lime_Char
	{
	public:
		ENGINE_DLL_API Lime_Char();
		ENGINE_DLL_API Lime_Char(char value);

		//Set functions
		ENGINE_DLL_API void SetCharacter(char value);
		ENGINE_DLL_API void SetColor(glm::vec3 color);
		ENGINE_DLL_API void SetColor(float red, float green, float blue);
		ENGINE_DLL_API void SetOpacity(float value);
		ENGINE_DLL_API void SetLength(float length);
		ENGINE_DLL_API void SetWidth(float width);
		ENGINE_DLL_API void SetPosition(float x, float y, float z);
		ENGINE_DLL_API void Move(float x, float y);

		//Get functions
		ENGINE_DLL_API Shapes::Square GetModel();
		ENGINE_DLL_API float GetLength();
		ENGINE_DLL_API float GetWidth();

	private:
		void SetTexture();
		void SetUVCoords();

		//Static variables
		static Graphics::TextureID m_textureID;

		//Local variables
		char m_char;
		float m_length;
		float m_width;
		Shapes::Square m_glyph;
	};

}
