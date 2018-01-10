#pragma once
#include "WindowsAdditionals.h"

namespace Graphics
{
	class Line
	{
	public:
		__engine_decl Line();
		__engine_decl Line(D2D1_POINT_2F p1, D2D1_POINT_2F p2);

		//Setters
		__engine_decl void SetBrush(int ID);
		__engine_decl void SetPointOne(D2D1_POINT_2F p1);
		__engine_decl void SetPointTwo(D2D1_POINT_2F p2);
		__engine_decl void SetPoints(D2D1_POINT_2F p1, D2D1_POINT_2F p2);
		__engine_decl void SetStrokeWidth(float val);

		//Getters
		__engine_decl D2D1_POINT_2F GetPointOne()const;
		__engine_decl D2D1_POINT_2F GetPointTwo()const;
		__engine_decl int GetBrush()const;
		__engine_decl float GetStrokeWidth()const;

	private:

		D2D1_POINT_2F m_point1;
		D2D1_POINT_2F m_point2;
		float m_strokeWidth;
		int m_brush;
	};


}