#include "Line.h"



Graphics::Line::Line()
{
	SetPointOne(D2D1::Point2F());
	SetPointTwo(D2D1::Point2F());
	m_strokeWidth = 0.5f;
}

Graphics::Line::Line(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
	SetPoints(p1, p2);
	m_strokeWidth = 0.5f;
}

void Graphics::Line::SetBrush(std::string uniqueName)
{
	m_brush = uniqueName;
}

void Graphics::Line::SetPointOne(D2D1_POINT_2F p1)
{
	m_point1 = p1;
}

void Graphics::Line::SetPointTwo(D2D1_POINT_2F p2)
{
	m_point2 = p2;
}

void Graphics::Line::SetPoints(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
	m_point1 = p1;
	m_point2 = p2;
}

void Graphics::Line::SetStrokeWidth(float val)
{
	m_strokeWidth = val;
}

D2D1_POINT_2F Graphics::Line::GetPointOne()const
{
	return m_point1;
}

D2D1_POINT_2F Graphics::Line::GetPointTwo()const
{
	return m_point2;
}

std::string Graphics::Line::GetBrushName()const
{
	return m_brush;
}

float Graphics::Line::GetStrokeWidth()const
{
	return m_strokeWidth;
}
