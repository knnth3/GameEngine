#include "Shape_Square.h"

Graphics::Square::Square()
{
	SetWidth(0.0f);
	SetHeight(0.0f);
}

Graphics::Square::Square(float width, float height)
{
	SetDimensions(width, height);
}

Graphics::Square::Square(float width, float height, float x, float y)
{

	SetSettings(width, height, x, y);
}

float Graphics::Square::GetWidth()const
{
	return m_width;
}

float Graphics::Square::GetHeight()const
{
	return m_height;
}

glm::vec2 Graphics::Square::GetPosition()const
{
	return m_position;
}

D2D1_RECT_F Graphics::Square::GetRectBounds()const
{
	return m_rect;
}

std::string Graphics::Square::GetBrushName()const
{
	return m_brush;
}

void Graphics::Square::SetBrush(std::string uniqueName)
{
	m_brush = uniqueName;
}

void Graphics::Square::SetSettings(float width, float height, float x, float y)
{
	m_width = width;
	m_height = height;
	m_position = { x, y };
	CreateRectBounds();
}

void Graphics::Square::SetDimensions(float width, float height)
{
	m_width = width;
	m_height = height;
	CreateRectBounds();
}

void Graphics::Square::SetWidth(float val)
{
	m_width = val;
	CreateRectBounds();
}

void Graphics::Square::SetHeight(float val)
{
	m_height = val;
	CreateRectBounds();
}

void Graphics::Square::SetPosition(glm::vec2 pos)
{
	m_position = pos;
	CreateRectBounds();
}

void Graphics::Square::SetPosition(float x, float y)
{
	m_position = { x, y };
	CreateRectBounds();
}

void Graphics::Square::CreateRectBounds()
{
	m_rect = D2D1::RectF(
		m_position.x,
		m_position.y,
		m_width,
		m_height
	);
}
