#include "Console.h"
#include "BrushManager.h"

using namespace std;
using namespace Engine;

#define CONSLE_BG "WINCONSOLEBG"

Console::Console(int maxLogs, float width, float height):
	m_maxLogs(maxLogs)
{
	m_width = width;
	m_height = height;
	m_textHeight = 30;
	m_currentIndex = maxLogs - 1;
	m_messages = new Text*[maxLogs];
	for (int x = 0; x < m_maxLogs; x++)
	{
		m_messages[x] = new Text();
		m_messages[x]->SetBounds(m_width, m_textHeight);
	}
	m_textureID = BrushManager::CreateNewBrush(glm::vec4(0.0f, 0.0f, 0.0f, 0.3f));
	m_background.SetDimensions(m_width, (m_textHeight * m_maxLogs) + m_textHeight);
	m_background.SetBrush(m_textureID, BRUSH_TEXTURE_DEFAULT);
}

Console::~Console()
{
	for (int x = 0; x < m_maxLogs; x++)
	{
		delete m_messages[x];
	}
	delete[] m_messages;
	BrushManager::DeleteBrush(m_textureID);
}

void Console::Log(const std::string & message)
{
	float currentHeight = float(m_textHeight * m_maxLogs);
	int currentIndex = 0;
	Text* temp = m_messages[0];
	m_messages[0] = m_messages[m_maxLogs - 1];
	*m_messages[0] = message;
	m_messages[0]->SetPosition(0.0f, currentHeight);
	currentHeight -= m_textHeight;
	for (int x = 0; x < m_maxLogs; x++)
	{
		if (x == m_maxLogs - 1)
			continue;

		Text* next = m_messages[x + 1];
		m_messages[x + 1] = temp;
		m_messages[x + 1]->SetPosition(0.0f, currentHeight);
		currentHeight -= m_textHeight;
		temp = next;
		next = nullptr;
	}
}

Square & Engine::Console::GetBackground()
{
	return m_background;
}

const Text & Engine::Console::At(int index) const
{
	if (index > m_currentIndex || index < 0)
		throw std::out_of_range("Index out of range");

	return *m_messages[index];
}

int Engine::Console::GetSize() const
{
	return m_maxLogs;
}

float Engine::Console::GetTextHeight() const
{
	return m_textHeight;
}

void Engine::Console::SetDimensions(float width, float height)
{
	m_width = width;
	m_height = height;
	m_background.SetDimensions(width, (m_textHeight * m_maxLogs) + m_textHeight);
}

void Engine::Console::SetTextHeight(float value)
{
	for (int index = 0; index < m_maxLogs; index++)
	{
		m_messages[index]->SetBounds(m_width, value);
	}
}
