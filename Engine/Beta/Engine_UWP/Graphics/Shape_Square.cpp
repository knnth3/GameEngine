#include "Shape_Square.h"

Graphics::Square::Square()
{
	SetDefaultSettings();
	SetPosition(0.0f, 0.0f);
	SetDimensions(0.0f, 0.0f);
}

Graphics::Square::Square(float width, float height)
{
	SetDefaultSettings();
	SetPosition(0.0f, 0.0f);
	SetDimensions(width, height);
}

Graphics::Square::Square(float width, float height, float x, float y)
{
	SetDefaultSettings();
	SetPosition(x, y);
	SetDimensions(width, height);
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

D2D1_RECT_F Graphics::Square::GetRect() const
{
	D2D1_RECT_F r;
	r.left = m_position.x;
	r.top = m_position.y;
	r.right = m_position.x + m_width;
	r.bottom = m_position.y + m_height;
	return r;
}

int Graphics::Square::GetBrush()const
{
	return m_brush;
}

Graphics::BRUSH_TEXTURE_TYPE Graphics::Square::GetBrushType() const
{
	return m_brushType;
}

glm::vec4 Graphics::Square::GetColor() const
{
	return m_color;
}

D2D1_RECT_F Graphics::Square::GetSourceRect() const
{
	D2D1_RECT_F r;
	float newWidth = GetImageAtlasDivisonWidth();
	float newHeight = GetImageAtlasDivisonHeight();

	r.left = (m_atlasPosX * newWidth);
	r.top = (m_atlasPosY * newHeight);
	r.right = (m_atlasPosX * newWidth) + newWidth;
	r.bottom = (m_atlasPosY * newHeight) + newHeight;
	return r;
}

float Graphics::Square::GetImageAtlasDivisonWidth()const
{
	return (m_imageWidth / m_atlasDivsX);
}

float Graphics::Square::GetImageAtlasDivisonHeight()const
{
	return (m_imageHeight / m_atlasDivsY);
}

void Graphics::Square::SetColor(glm::vec4 color)
{
	m_color = color;
}

void Graphics::Square::SetBrush(int ID, BRUSH_TEXTURE_TYPE type)
{
	glm::vec2 dims = glm::vec2(0.0f, 0.0f);
	if (type != BRUSH_TEXTURE_DEFAULT)
		BrushManager::GetImageDimensions(ID, dims);

	m_imageWidth = dims.x;
	m_imageHeight = dims.y;
	m_brush = ID;
	m_brushType = type;
}

void Graphics::Square::SetDimensions(float width, float height)
{
	m_width = width;
	m_height = height;
}

void Graphics::Square::SetWidth(float val)
{
	m_width = val;
}

void Graphics::Square::SetHeight(float val)
{
	m_height = val;
}

void Graphics::Square::SetPosition(glm::vec2 pos)
{
	m_position = pos;
}

void Graphics::Square::SetPosition(float x, float y)
{
	m_position = glm::vec2(x, y);
}

void Graphics::Square::SetImageAtlasDivisons(int x, int y)
{

	if (x > 0 && y > 0)
	{
		m_atlasDivsX = x;
		m_atlasDivsY = y;
	}
}

void Graphics::Square::SetImageAtlasPosition(int x, int y)
{
	m_atlasPosX = x;
	m_atlasPosY = y;
}

void Graphics::Square::SetDefaultSettings()
{
	SetImageAtlasPosition(0, 0);
	SetImageDimensions(0.0f, 0.0f);
	SetImageAtlasDivisons(1, 1);
	SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetBrush(-1, BRUSH_TEXTURE_TYPE::BRUSH_TEXTURE_DEFAULT);
}

void Graphics::Square::SetImageDimensions(float width, float height)
{
	m_imageWidth = width;
	m_imageHeight = height;
}
