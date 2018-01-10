#pragma once
#include "MouseInput.h"


namespace Graphics
{
	class Input
	{
	public:
		__engine_decl Input();
		__engine_decl void AttatchKeyboard(const std::shared_ptr<InputDevice>& device);
		__engine_decl const std::shared_ptr<MouseInput>& GetMouse();
		__engine_decl const std::shared_ptr<InputDevice>& GetKeyboard();
		__engine_decl bool KeyDown(int Key)const;
		__engine_decl bool KeyPressed(int Key)const;

	private:
		std::shared_ptr<MouseInput> m_mouse;
		std::shared_ptr<InputDevice> m_keyboard;
	};
}

