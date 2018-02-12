#include "pch.h"
#include "Button.h"
#include <Lime_Engine\EngineResources.h>

using namespace Graphics;
using namespace LIME_ENGINE;

Button::Button()
{
	m_bIsClicked = false;
	m_defaultBrushType = BRUSH_TEXTURE_DEFAULT;
	m_hoveredBrushType = BRUSH_TEXTURE_DEFAULT;
}

Button::Button(float width, float height, float x, float y):
	Square(width, height, x, y)
{

	m_bIsClicked = false;
	m_defaultBrushType = BRUSH_TEXTURE_DEFAULT;
	m_hoveredBrushType = BRUSH_TEXTURE_DEFAULT;
}

void Button::SetHoveredBrush(int brushID, BRUSH_TEXTURE_TYPE type)
{
	m_hoveredBrush = brushID;
	m_hoveredBrushType = type;
}

void Button::SetDefaultBrush(int brushID, BRUSH_TEXTURE_TYPE type)
{
	m_defaultBrush = brushID;
	m_defaultBrushType = type;
}

//Checks for collision based off 2D of bounding box
void Button::Update(bool buttonPressed)
{
	auto input = EngineResources::GetInputManager();
	auto mousepos = input->GetMouseCoords();
	auto boundingBox = this->GetRect();
	if (mousepos.x <= boundingBox.right && mousepos.x >= boundingBox.left && mousepos.y <= boundingBox.bottom && mousepos.y >= boundingBox.top)
	{
		this->SetBrush(m_hoveredBrush, m_hoveredBrushType);
		if (buttonPressed)
			m_bIsClicked = true;
	}
	else
	{
		this->SetBrush(m_defaultBrush, m_defaultBrushType);
		m_bIsClicked = false;
	}
}

bool Button::IsClicked()
{
	bool result = m_bIsClicked;
	m_bIsClicked = false;
	return result;
}