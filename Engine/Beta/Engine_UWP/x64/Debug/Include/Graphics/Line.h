#pragma once
#include "WindowsAdditionals.h"

namespace Graphics
{
	class Line
	{
	public:
		GRAPHICS_DLL_API Line();
		GRAPHICS_DLL_API Line(D2D1_POINT_2F p1, D2D1_POINT_2F p2);

		//Setters
		GRAPHICS_DLL_API void SetBrush(int ID);
		GRAPHICS_DLL_API void SetPointOne(D2D1_POINT_2F p1);
		GRAPHICS_DLL_API void SetPointTwo(D2D1_POINT_2F p2);
		GRAPHICS_DLL_API void SetPoints(D2D1_POINT_2F p1, D2D1_POINT_2F p2);
		GRAPHICS_DLL_API void SetStrokeWidth(float val);

		//Getters
		GRAPHICS_DLL_API D2D1_POINT_2F GetPointOne()const;
		GRAPHICS_DLL_API D2D1_POINT_2F GetPointTwo()const;
		GRAPHICS_DLL_API int GetBrush()const;
		GRAPHICS_DLL_API float GetStrokeWidth()const;

	private:

		D2D1_POINT_2F m_point1;
		D2D1_POINT_2F m_point2;
		float m_strokeWidth;
		int m_brush;
	};


}