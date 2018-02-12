#pragma once
#include "WindowsAdditionals.h"
#include "BrushManager.h"
#include <glm\glm.hpp>

namespace Engine
{

	class Square
	{
	public:
		__engine_decl Square();
		__engine_decl Square(float width, float height);
		__engine_decl Square(float width, float height, float x, float y);

		//Get
		__engine_decl float GetWidth()const;
		__engine_decl float GetHeight()const;
		__engine_decl glm::vec2 GetPosition()const;
		__engine_decl D2D1_RECT_F GetRect()const;
		__engine_decl int GetBrush()const;
		__engine_decl BRUSH_TEXTURE_TYPE GetBrushType()const;
		__engine_decl glm::vec4 GetColor()const;
		__engine_decl D2D1_RECT_F GetSourceRect()const;
		__engine_decl float GetImageDivisionWidth()const;
		__engine_decl float GetImageDivisionHeight()const;
		__engine_decl int GetImageAtlasDivisionWidth()const;
		__engine_decl int GetImageAtlasDivisionHeight()const;
		__engine_decl int GetImageAtlasPositionX()const;
		__engine_decl int GetImageAtlasPositionY()const;

		//Set
		__engine_decl void SetColor(glm::vec4 color);
		__engine_decl void SetBrush(int ID, BRUSH_TEXTURE_TYPE type);
		__engine_decl void SetDimensions(float width, float height);
		__engine_decl void SetWidth(float val);
		__engine_decl void SetHeight(float val);
		__engine_decl void SetPosition(glm::vec2 pos);
		__engine_decl void SetPosition(float x, float y);
		__engine_decl void SetImageAtlasDivisons(int x, int y);
		__engine_decl void SetImageAtlasPosition(int x, int y);

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