#include "Button.h"
#include <Lime\TextureManager.h>


Button::Button(float x, float y, float length, float width)
{
	auto texture = Lime::TextureManager::CreateNewTexture(L"Assets/textures/buttons/Button_unselected.dds");

	m_length = length;
	m_width = width;
	m_pos = { x, y };
	m_model = std::make_shared<Lime::Model::Model2D>(m_pos, length, width);
	m_model->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_model->SetTexture(texture);
	m_bIsClicked = false;
	m_bReleased = false;
}

void Button::Update(glm::vec2 mousePos, bool mouseButtonDown)
{
	float maxX = m_pos.x + (m_length *0.5f);
	float minX = m_pos.x - (m_length *0.5f);
	float maxY = m_pos.y + (m_width * 0.5f);
	float minY = m_pos.y - (m_width * 0.5f);
	if (mousePos.x < maxX && mousePos.x > minX && mousePos.y <= maxY && mousePos.y >= minY)
	{
		m_model->SetColor(0.0f, 1.0f, 0.0f);
		if (m_bIsClicked)
		{
			if (!mouseButtonDown)
			{
				m_bIsClicked = false;
				m_bReleased = true;
			}
			else
				m_model->SetColor(0.0f, 0.0f, 0.0f);
		}
		else if (mouseButtonDown)
		{
			m_bIsClicked = true;
		}

	}
	else
	{
		m_model->SetColor(1.0f, 1.0f, 1.0f);
		m_bIsClicked = false;
	}
}

void Button::Draw()
{
	m_model->Draw();
}

std::shared_ptr<Lime::Model::Model2D>& Button::GetModel()
{
	return m_model;
}

bool Button::IsClicked()
{
	bool result = m_bReleased;
	m_bReleased = false;
	return result;
}
