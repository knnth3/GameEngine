#include "Lime_String.h"



using namespace LIME_ENGINE;
LIME_ENGINE::Lime_String::Lime_String()
{
	m_str = "";
	SetPosition(0.0f, 0.0f);
	SetSpacing(1.0f);
}

LIME_ENGINE::Lime_String::Lime_String(std::string str)
{
	m_str = str;
	SetPosition(0.0f, 0.0f);
	SetSpacing(1.0f);
	CreateCharacters();
}

void LIME_ENGINE::Lime_String::SetColor(glm::vec3 color)
{
	SetColor(color.r, color.g, color.b);
}

void LIME_ENGINE::Lime_String::SetColor(float red, float green, float blue)
{
	for (auto x : m_characters)
		x.SetColor(red, green, blue);
}

void LIME_ENGINE::Lime_String::SetOpacity(float opacity)
{
	for (auto x : m_characters)
		x.SetOpacity(opacity);
}

void LIME_ENGINE::Lime_String::SetPosition(float x, float y)
{
	float xDifference = x - m_xPos;
	float yDifference = y - m_yPos;
	Move(xDifference, yDifference);
}

void LIME_ENGINE::Lime_String::SetPosition(glm::vec2 pos)
{
	SetPosition(pos.x, pos.y);
}

void LIME_ENGINE::Lime_String::Move(float xDifference, float yDifference)
{
	if (xDifference + yDifference != 0.0f)
	{
		m_xPos += xDifference;
		m_yPos += yDifference;
		for (auto glyph : m_characters)
		{
			glyph.Move(xDifference, yDifference);
		}
	}
}

void LIME_ENGINE::Lime_String::SetSpacing(float val)
{
	m_spacing = val;
}

void LIME_ENGINE::Lime_String::operator=(const std::string str)
{
	if (this->m_str.compare(str) != 0)
	{
		this->m_str = str;
		this->CreateCharacters();
	}
}

Lime_Char & LIME_ENGINE::Lime_String::operator[](const size_t index)
{
	return m_characters[index];
}

void LIME_ENGINE::Lime_String::Draw(std::shared_ptr<Graphics::GraphicsDevice>& device)
{
	for (auto x : m_characters)
		device->Draw(x.GetModel());
}

void LIME_ENGINE::Lime_String::CreateCharacters()
{
	m_characters.resize(m_str.size());
	float prevCharLength = 0.0f;
	for (size_t index = 0; index < m_characters.size(); index++)
	{
		m_characters[index].SetCharacter(m_str[index]);
		m_characters[index].SetPosition(m_xPos + prevCharLength, m_yPos, 0.0f);
		prevCharLength += m_characters[index].GetLength() + m_spacing;
	}
}
