#include "Lime_Char.h"
#include "Graphics\DX11TextureManager.h"

using namespace LIME_ENGINE;
using namespace Shapes;
using namespace Graphics;

TextureID Lime_Char::m_textureID = -1;
LIME_ENGINE::Lime_Char::Lime_Char():
	m_length(15.0f),
	m_width(30.0f),
	m_glyph(m_length, m_width)
{
	SetCharacter('a');
}

LIME_ENGINE::Lime_Char::Lime_Char(char value):
	m_length(15.0f),
	m_width(30.0f),
	m_glyph(m_length, m_width)
{
	SetCharacter(value);
}

void LIME_ENGINE::Lime_Char::SetCharacter(char value)
{
	m_char = value;
	SetTexture();
	SetUVCoords();
}

void LIME_ENGINE::Lime_Char::SetColor(glm::vec3 color)
{
	SetColor(color.r, color.g, color.b);
}

void LIME_ENGINE::Lime_Char::SetColor(float red, float green, float blue)
{
	m_glyph.SetColor(red, green, blue);
}

void LIME_ENGINE::Lime_Char::SetOpacity(float value)
{
	m_glyph.SetOpacity(value);
}

void LIME_ENGINE::Lime_Char::SetLength(float length)
{
	m_length = length;
	m_glyph.SetLength(m_length);
}

void LIME_ENGINE::Lime_Char::SetWidth(float width)
{
	m_width = width;
	m_glyph.SetWidth(m_width);
}

void LIME_ENGINE::Lime_Char::SetPosition(float x, float y, float z)
{
	m_glyph.SetPosition(x, y, z);
}

void LIME_ENGINE::Lime_Char::Move(float x, float y)
{
	glm::vec3 pos = m_glyph.GetPosition();
	SetPosition(pos.x + x, pos.y + y, 0.0f);
}

Shapes::Square LIME_ENGINE::Lime_Char::GetModel()
{
	return m_glyph;
}

float LIME_ENGINE::Lime_Char::GetLength()
{
	return m_length;
}

float LIME_ENGINE::Lime_Char::GetWidth()
{
	return m_width;
}

void LIME_ENGINE::Lime_Char::SetTexture()
{
	if (m_textureID == -1)
		m_textureID = DX11TextureManager::CreateNewTexture
		("EngineAssets/textures/SpriteSheetx200.dds");

	m_glyph.SetTexture(m_textureID);
}

void LIME_ENGINE::Lime_Char::SetUVCoords()
{
	uint16_t glyph = (m_char - 31) + 5;
	uint16_t posy;
	uint16_t posx = glyph % 20;
	if (posx == 0)
	{
		posx = glyph;
		posy = glyph / 20;
	}
	else
	{
		posy = (glyph / 20) + 1;
	}
	float xLen = 0.05f;
	float yLen = 0.2f;
	float length = (xLen*posx) - xLen;
	float width = (yLen*posy) - yLen;

	m_glyph.SetTextureBounds(xLen, yLen, length, width);
}
