#include "Input.h"


using namespace Engine;

Engine::Input::Input()
{
	m_mouse = std::make_shared<MouseInput>();
	m_keyboard = std::make_shared<InputDevice>();
}

void Engine::Input::AttatchKeyboard(const std::shared_ptr<InputDevice>& device)
{
	m_keyboard = device;
}

const std::shared_ptr<MouseInput>& Engine::Input::GetMouse()
{
	return m_mouse;
}

const std::shared_ptr<InputDevice>& Engine::Input::GetKeyboard()
{
	return m_keyboard;
}

bool Engine::Input::KeyDown(int Key)const
{
	return m_keyboard->ButtonDown(Key);
}

bool Engine::Input::KeyPressed(int Key)const
{
	return m_keyboard->ButtonPressed(Key);
}
