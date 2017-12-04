#include "Text.h"

Graphics::Text::Text()
{
	SetPosition(0.0f, 0.0f);
}

Graphics::Text::Text(std::wstring str, std::string style):
	Text()
{
	this->insert(this->begin(), str.begin(), str.end());
	SetStyle(style);
}

Graphics::Text::Text(std::string str, std::string style):
	Text()
{
	this->insert(this->begin(), str.begin(), str.end());
	SetStyle(style);
}

void Graphics::Text::SetStyle(std::string style)
{
	m_style = style;
}

void Graphics::Text::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
	m_translation = D2D1::Matrix3x2F::Translation(
		m_position.x,
		m_position.y
	);
}

void Graphics::Text::SetPosition(glm::vec2 pos)
{
	m_position = pos;
}

std::string Graphics::Text::GetStyleName()const
{
	return m_style;
}

glm::vec2 Graphics::Text::GetPosition()const
{
	return m_position;
}

D2D1::Matrix3x2F Graphics::Text::GetTranslation()const
{
	return m_translation;
}

void Graphics::Text::operator=(std::string & str)
{
	this->clear();
	this->insert(this->begin(), str.begin(), str.end());
}

void Graphics::Text::operator=(std::wstring & str)
{
	this->clear();
	this->insert(this->begin(), str.begin(), str.end());
}
