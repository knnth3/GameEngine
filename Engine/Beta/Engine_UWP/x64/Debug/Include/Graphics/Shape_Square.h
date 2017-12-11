#pragma once
#include "WindowsAdditionals.h"
#include "BrushManager.h"
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
		GRAPHICS_DLL_API D2D1_RECT_F GetRect()const;
		GRAPHICS_DLL_API int GetBrush()const;
		GRAPHICS_DLL_API BRUSH_TEXTURE_TYPE GetBrushType()const;
		GRAPHICS_DLL_API glm::vec4 GetColor()const;
		GRAPHICS_DLL_API D2D1_RECT_F GetSourceRect()const;
		GRAPHICS_DLL_API float GetImageAtlasDivisonWidth()const;
		GRAPHICS_DLL_API float GetImageAtlasDivisonHeight()const;

		//Set
		GRAPHICS_DLL_API void SetColor(glm::vec4 color);
		GRAPHICS_DLL_API void SetBrush(int ID, BRUSH_TEXTURE_TYPE type);
		GRAPHICS_DLL_API void SetDimensions(float width, float height);
		GRAPHICS_DLL_API void SetWidth(float val);
		GRAPHICS_DLL_API void SetHeight(float val);
		GRAPHICS_DLL_API void SetPosition(glm::vec2 pos);
		GRAPHICS_DLL_API void SetPosition(float x, float y);
		GRAPHICS_DLL_API void SetImageAtlasDivisons(int x, int y);
		GRAPHICS_DLL_API void SetImageAtlasPosition(int x, int y);

	private:
		void SetDefaultSettings();
		void SetImageDimensions(float width, float height);

		int m_brush;
		int m_atlasPosX;
		int m_atlasPosY;
		int m_atlasDivsX;
		int m_atlasDivsY;
		float m_width;
		float m_height;
		float m_imageWidth;
		float m_imageHeight;
		glm::vec2 m_position;
		glm::vec4 m_color;
		BRUSH_TEXTURE_TYPE m_brushType;
	};
}