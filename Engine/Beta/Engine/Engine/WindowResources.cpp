#include "WindowResources.h"

using namespace Graphics;
using namespace std;

#define GRAPHICS_INDEX 0
#define INPUT_INDEX 1
#define TIMER_INDEX 2

const string WindowResources::m_reserved[3] = { "GRAPHICS", "INPUT", "TIMER" };
universal_map WindowResources::m_resources;


void Graphics::WindowResources::SetGraphics(const universal_struct & graphics)
{
	if (!m_resources.Insert(m_reserved[GRAPHICS_INDEX], graphics))
	{
		m_resources.Remove(m_reserved[GRAPHICS_INDEX]);
		m_resources.Insert(m_reserved[GRAPHICS_INDEX], graphics);
	}
}

void Graphics::WindowResources::SetInput(const universal_struct & input)
{
	if (!m_resources.Insert(m_reserved[INPUT_INDEX], input))
	{
		m_resources.Remove(m_reserved[INPUT_INDEX]);
		m_resources.Insert(m_reserved[INPUT_INDEX], input);
	}
}

void Graphics::WindowResources::SetTimer(const universal_struct & timer)
{
	if (!m_resources.Insert(m_reserved[TIMER_INDEX], timer))
	{
		m_resources.Remove(m_reserved[TIMER_INDEX]);
		m_resources.Insert(m_reserved[TIMER_INDEX], timer);
	}
}

void Graphics::WindowResources::RemoveAddon(const std::string & key)
{
	if (IsReserved(key))
		return;

	m_resources.Remove(key);
}

void Graphics::WindowResources::Reset()
{
	m_resources.Clear();
}

bool Graphics::WindowResources::InsertAddon(const std::string & key, const universal_struct & obj)
{
	if (IsReserved(key))
		return false;

	return m_resources.Insert(key, obj);
}

std::shared_ptr<GraphicsDevice> Graphics::WindowResources::GetGraphics()
{
	auto graphics = m_resources[m_reserved[GRAPHICS_INDEX]];
	if(graphics)
		return graphics->As<GraphicsDevice>();

	return nullptr;
}

std::shared_ptr<Input> Graphics::WindowResources::GetInput()
{
	auto input = m_resources[m_reserved[INPUT_INDEX]];
	if (input)
		return input->As<Input>();

	return nullptr;
}

std::shared_ptr<Timer> Graphics::WindowResources::GetTimer()
{
	auto timer = m_resources[m_reserved[TIMER_INDEX]];
	if(timer)
		return timer->As<Timer>();

	return nullptr;
}

const universal_struct * Graphics::WindowResources::GetAddon(const std::string & key)
{
	return m_resources[key];
}

std::vector<std::string> Graphics::WindowResources::GetKeyList()
{
	return m_resources.List();
}

bool Graphics::WindowResources::IsReserved(const std::string & key)
{
	string uppercase = To_upper(key);
	for (auto& x : m_reserved)
	{
		if (uppercase.compare(x) == 0)
			return true;
	}

	return false;
}
