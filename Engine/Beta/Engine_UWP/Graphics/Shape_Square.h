#pragma once
#include "WindowsAdditionals.h"
#include <glm\glm.hpp>

namespace Graphics
{
	class Square
	{
	public:
		GRAPHICS_DLL_API Square();
		GRAPHICS_DLL_API Square(float width, float height);
		GRAPHICS_DLL_API Square(float width, float height, float x, float y);

		//Get
		GRAPHICS_DLL_API float GetWidth()const;
		GRAPHICS_DLL_API float GetHeight()const;
		GRAPHICS_DLL_API glm::vec2 GetPosition()const;
		GRAPHICS_DLL_API D2D1_RECT_F GetRectBounds()const;
		GRAPHICS_DLL_API std::string GetBrushName()const;

		//Set
		GRAPHICS_DLL_API void SetBrush(std::string uniqueName);
		GRAPHICS_DLL_API void SetSettings(float width, float height, float x, float y);
		GRAPHICS_DLL_API void SetDimensions(float width, float height);
		GRAPHICS_DLL_API void SetWidth(float val);
		GRAPHICS_DLL_API void SetHeight(float val);
		GRAPHICS_DLL_API void SetPosition(glm::vec2 pos);
		GRAPHICS_DLL_API void SetPosition(float x, float y);

	private:
		void CreateRectBounds();

		float m_width;
		float m_height;
		glm::vec2 m_position;
		D2D1_RECT_F m_rect;
		std::string m_brush;
	};
}