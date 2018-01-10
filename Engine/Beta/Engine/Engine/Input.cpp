#include "Input.h"


using namespace Graphics;

Graphics::Input::Input()
{
	m_mouse = std::make_shared<MouseInput>();
	m_keyboard = std::make_shared<InputDevice>();
}

void Graphics::Input::AttatchKeyboard(const std::shared_ptr<InputDevice>& device)
{
	m_keyboard = device;
}

const std::shared_ptr<MouseInput>& Graphics::Input::GetMouse()
{
	return m_mouse;
}

const std::shared_ptr<InputDevice>& Graphics::Input::GetKeyboard()
{
	return m_keyboard;
}

bool Graphics::Input::KeyDown(int Key)const
{
	return m_keyboard->ButtonDown(Key);
}

bool Graphics::Input::KeyPressed(int Key)const
{
	return m_keyboard->ButtonPressed(Key);
}
