#pragma once
#include "Lime_Char.h"


namespace LIME_ENGINE
{
	class Lime_String
	{
	public:
		ENGINE_DLL_API Lime_String();
		ENGINE_DLL_API Lime_String(std::string str);

		//Set functions
		ENGINE_DLL_API void SetColor(glm::vec3 color);
		ENGINE_DLL_API void SetColor(float red, float green, float blue);
		ENGINE_DLL_API void SetOpacity(float opacity);
		ENGINE_DLL_API void SetPosition(float x, float y);
		ENGINE_DLL_API void SetPosition(glm::vec2 pos);
		ENGINE_DLL_API void Move(float xDifference, float yDifference);
		ENGINE_DLL_API void SetSpacing(float val);

		//Operator overload functions
		ENGINE_DLL_API void operator= (const std::string str);
		ENGINE_DLL_API Lime_Char& operator[] (const size_t index);

		//Draw function
		ENGINE_DLL_API void Draw(std::shared_ptr<Graphics::GraphicsDevice>& device);

	private:
		void CreateCharacters();

		float m_xPos;
		float m_yPos;
		float m_spacing;
		std::string m_str;
		std::vector<Lime_Char> m_characters;
	};

}
