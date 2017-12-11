#include "Console.h"
#include "EngineResources.h"
#include <Graphics\BrushManager.h>

#define MAX_STRINGS 7

using namespace Graphics;

LIME_ENGINE::Console::Console()
{
	m_bActive = false;
	EngineResources::GetGraphicsDevice()->GetWindowDimensions(m_width, m_height);
	m_height *= 0.5f;
	int ID = BrushManager::CreateNewBrush(glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
	m_textBrush = BrushManager::CreateNewBrush(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_background = Square(m_width, m_height, 0.0f, 0.0f);
	m_background.SetBrush(ID, BRUSH_TEXTURE_DEFAULT);
	m_background.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
	m_fpsText.SetBounds(200.0f, 100.0f);
}

void LIME_ENGINE::Console::Log(std::wstring text)
{
	Log(text, m_textBrush);
}

void LIME_ENGINE::Console::Log(std::wstring text, int brushID)
{
	//Thread-Safe yet
	Text str;
	str = text;
	str.SetBounds(m_width, 40);
	str.SetPosition(0.0f, m_height - str.GetBounds().y);
	str.SetBrush(brushID);
	std::lock_guard<std::mutex> lock(m_mutex);
	Push_Back(str);
}

void LIME_ENGINE::Console::Update()
{
	static double timer = 0.0;
	if(m_strings.size() > 0)
		timer += EngineResources::GetTimer()->GetElapsedSeconds();

	bool pressed = EngineResources::GetInputManager()->KeyPressed(m_openButton);
	if (pressed)
	{
		Switch();
	}

	std::lock_guard<std::mutex> lock(m_mutex);
	if (timer > 5.0)
	{
		m_strings.pop_front();
		ReorderStrings();
		timer = 0.0;
	}
}

void LIME_ENGINE::Console::Render()
{
	if (m_bActive)
	{
		auto fps = EngineResources::GetTimer()->GetFramesPerSecond();
		auto graphics = EngineResources::GetGraphicsDevice();
		m_fpsText = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";
		graphics->Draw(m_background);
		graphics->Draw(m_fpsText);
		for (auto str : m_strings)
			graphics->Draw(str);
	}
}

void LIME_ENGINE::Console::Switch()
{
	m_bActive = !m_bActive;
}

void LIME_ENGINE::Console::UpdateDimensions()
{
	EngineResources::GetGraphicsDevice()->GetWindowDimensions(m_width, m_height);
	m_height *= 0.5f;
	m_background.SetDimensions(m_width, m_height);
	int count = (int)m_strings.size();
	for (auto& str : m_strings)
		str.SetPosition(0.0f, m_height - (str.GetBounds().y * count--));
	m_fpsText.SetPosition(m_width - 200.0f, (m_height*2.0f) - 100.0f);
}

void LIME_ENGINE::Console::SetTextColor(glm::vec4 color)
{
	m_textBrush = BrushManager::CreateNewBrush(color);
}

void LIME_ENGINE::Console::SetBGColor(glm::vec4 color)
{
	m_background.SetBrush(BrushManager::CreateNewBrush(color), BRUSH_TEXTURE_DEFAULT);
}

void LIME_ENGINE::Console::SetToggleButton(int code)
{
	m_openButton = code;
}

void LIME_ENGINE::Console::Push_Back(Text& newStr)
{
	m_strings.emplace_back(newStr);
	if (m_strings.size() > MAX_STRINGS)
		m_strings.pop_front();

	ReorderStrings();
}

void LIME_ENGINE::Console::ReorderStrings()
{
	float newPos = m_height;
	for (auto& x : m_strings)
	{
		newPos -= x.GetBounds().y;
		x.SetPosition(0.0f, newPos);
	}
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
