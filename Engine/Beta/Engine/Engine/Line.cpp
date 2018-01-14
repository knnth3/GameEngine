#include "Line.h"



Engine::Line::Line()
{
	SetPoints(D2D1::Point2F(), D2D1::Point2F());
	SetStrokeWidth(0.5f);
	SetBrush(-1);
}

Engine::Line::Line(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
	SetPoints(p1, p2);
	SetStrokeWidth(0.5f);
	SetBrush(-1);
}

void Engine::Line::SetBrush(int ID)
{
	m_brush = ID;
}

void Engine::Line::SetPointOne(D2D1_POINT_2F p1)
{
	m_point1 = p1;
}

void Engine::Line::SetPointTwo(D2D1_POINT_2F p2)
{
	m_point2 = p2;
}

void Engine::Line::SetPoints(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
	m_point1 = p1;
	m_point2 = p2;
}

void Engine::Line::SetStrokeWidth(float val)
{
	m_strokeWidth = val;
}

D2D1_POINT_2F Engine::Line::GetPointOne()const
{
	return m_point1;
}

D2D1_POINT_2F Engine::Line::GetPointTwo()const
{
	return m_point2;
}

int Engine::Line::GetBrush()const
{
	return m_brush;
}

float Engine::Line::GetStrokeWidth()const
{
	return m_strokeWidth;
}
