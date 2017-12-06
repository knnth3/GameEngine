#include "Console.h"


using namespace Graphics;

LIME_ENGINE::Console::Console(std::shared_ptr<GraphicsDevice>& graphicsDevice)
{
	m_bActive = false;
	m_graphics = graphicsDevice;
	m_graphics->GetWindowDimensions(m_width, m_height);
	m_height *= 0.5f;
	m_background = Square(m_width, m_height, 0.0f, 0.0f);
	CreateCommonBrushes();
	m_background.SetBrush("Console");
	m_background.SetBitmapOpacity(0.5f);
}

void LIME_ENGINE::Console::Log(std::wstring text)
{
	Log(text, "Console_Text");
}

void LIME_ENGINE::Console::Log(std::wstring text, std::string brushName)
{
	//Thread-Safe yet
	Text str;
	str = text;
	str.SetBounds(m_width, 40);
	str.SetPosition(0.0f, m_height - str.GetBounds().y);
	str.SetBrush(brushName);
	std::lock_guard<std::mutex> lock(m_mutex);
	ShiftOtherStrings();
	m_strings.emplace_back(str);
}

void LIME_ENGINE::Console::Update()
{
}

void LIME_ENGINE::Console::Render()
{
	if (m_bActive && m_graphics)
	{
		m_graphics->Draw(m_background);
		for (auto str : m_strings)
			m_graphics->Draw(str);
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

void LIME_ENGINE::Console::UpdateDimensions(float width, float height)
{
	m_width = width;
	m_height = height * 0.5f;
	m_background.SetDimensions(m_width, m_height);
	int count = (int)m_strings.size();
	for (auto& str : m_strings)
		str.SetPosition(0.0f, m_height - (str.GetBounds().y * count--));
}

void LIME_ENGINE::Console::SetTextColor(glm::vec4 color)
{
	m_graphics->CreateNew2DBrush("Console_Text", color);
}

void LIME_ENGINE::Console::SetBGColor(glm::vec4 color)
{
	m_graphics->CreateNew2DBrush("Console", color);
}

void LIME_ENGINE::Console::CreateCommonBrushes()
{
	m_graphics->CreateNew2DBrush("Console", glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
	m_graphics->CreateNew2DBrush("Console_Text", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void LIME_ENGINE::Console::ShiftOtherStrings()
{
	if (m_strings.size() == 10)
		m_strings.pop_front();

	for (auto& x : m_strings)
		x.SetPosition(0.0f, x.GetPosition().y - x.GetBounds().y);
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
