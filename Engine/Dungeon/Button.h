#pragma once
#include "Shared.h"
#include <Lime\Model2D.h>
#include <Lime\DX11Graphics.h>

class Button
{
public:
	Button(float x, float y, float length, float width);
	void Update(glm::vec2 mousePos, bool mouseButtonDown);
	void Draw();

	std::shared_ptr<Lime::Model::Model2D>& GetModel();
	bool IsClicked();

private:


	bool m_bIsClicked;
	bool m_bReleased;
	glm::vec2 m_pos;
	float m_length;
	float m_width;
	std::shared_ptr<Lime::Model::Model2D> m_model;
};

