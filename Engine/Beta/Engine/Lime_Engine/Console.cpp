#include "Console.h"
#include "Square.h"


using namespace Graphics;
LIME_ENGINE::Console::Console(const uint16_t windowWidth)
{
	m_bActive = false;
	m_length = float(windowWidth);
	m_width = 330; //About 11 characters in width
	m_model = Shapes::Square(m_length, m_width);
	m_model.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
}

LIME_ENGINE::Console::~Console()
{
}

void LIME_ENGINE::Console::Log(std::string text, float opacity)
{
	Log(text, LOG_WHITE, opacity);
}

void LIME_ENGINE::Console::Log(std::string text, ENGINE_COLORS color, float opacity)
{
	Log(text, EvaluateColor(color), opacity);
}

void LIME_ENGINE::Console::Log(std::string text, glm::vec3 color, float opacity)
{
	Log(text, color.r, color.g, color.b, opacity);
}

void LIME_ENGINE::Console::Log(std::string text, float red, float green, float blue, float opacity)
{
	//Thread-Safe
	float alpha_val = opacity * m_model.GetColor().a;
	Lime_String str = text;
	str.SetColor(red, green, blue);
	str.SetOpacity(alpha_val);
	str.SetPosition(0.0f, m_width - str[0].GetWidth());
	std::lock_guard<std::mutex> lock(m_mutex);
	ShiftOtherStrings();
	m_strings.emplace_back(str);

}

void LIME_ENGINE::Console::RenderConsole(std::shared_ptr<Graphics::GraphicsDevice>& device)
{
	if (m_bActive)
	{
		for (auto str : m_strings)
			str.Draw(device);
		device->Draw(m_model);
	}
}

void LIME_ENGINE::Console::Activate(bool value)
{
	m_bActive = value;
}

void LIME_ENGINE::Console::Switch()
{
	m_bActive = !m_bActive;
}

void LIME_ENGINE::Console::SetBackground(Graphics::TextureID id)
{
	m_model.SetTexture(id);
}

void LIME_ENGINE::Console::SetOpacity(float value)
{
	m_model.SetOpacity(value);
}

void LIME_ENGINE::Console::SetColor(ENGINE_COLORS color)
{
	SetColor(EvaluateColor(color));
}

void LIME_ENGINE::Console::SetColor(glm::vec3 color)
{
	SetColor(color.r, color.g, color.b);
}

void LIME_ENGINE::Console::SetColor(float red, float green, float blue)
{
	m_model.SetColor(red, green, blue);
}

void LIME_ENGINE::Console::ShiftOtherStrings()
{
	if (m_strings.size() == 10)
		m_strings.pop_front();

	for (auto x : m_strings)
		x.Move(0.0f, -x[0].GetWidth());
}

glm::vec3 LIME_ENGINE::Console::EvaluateColor(ENGINE_COLORS color)
{
	glm::vec3 evaluated_color(0.0f);
	switch (color)
	{
	case LIME_ENGINE::LOG_WHITE:
		evaluated_color.r = 1.0f;
		evaluated_color.g = 1.0f;
		evaluated_color.b = 1.0f;
		break;
	case LIME_ENGINE::LOG_RED:
		evaluated_color.r = 1.0f;
		break;
	case LIME_ENGINE::LOG_BLUE:
		evaluated_color.b = 1.0f;
		break;
	case LIME_ENGINE::LOG_GREEN:
		evaluated_color.g = 1.0f;
		break;
	case LIME_ENGINE::LOG_BLACK:
	default:
		break;
	}

	return evaluated_color;
}
