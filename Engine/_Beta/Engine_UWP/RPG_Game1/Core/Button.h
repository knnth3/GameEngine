#pragma once
#include <Graphics\Shape_Square.h>

//Simple logical implementation of a button for the Lime Engine
//uses a 2D Model to represent the button visual

class Button:
	public Graphics::Square
{
public:
	Button();
	Button(float width, float height, float x, float y);
	void SetHoveredBrush(int brushID, Graphics::BRUSH_TEXTURE_TYPE type);
	void SetDefaultBrush(int brushID, Graphics::BRUSH_TEXTURE_TYPE type);
	void Update(bool buttonPressed);
	bool IsClicked();

private:

	bool m_bIsClicked;
	int m_hoveredBrush;
	int m_defaultBrush;
	Graphics::BRUSH_TEXTURE_TYPE m_hoveredBrushType;
	Graphics::BRUSH_TEXTURE_TYPE m_defaultBrushType;
};

