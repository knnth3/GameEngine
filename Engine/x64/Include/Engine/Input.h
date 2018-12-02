#pragma once
#include "MouseInput.h"


namespace Engine
{
	typedef std::shared_ptr<MouseInput> EMouse;
	typedef std::shared_ptr<InputDevice> EKeyboard;

	class Input
	{
	public:
		__engine_decl Input();
		__engine_decl void AttatchKeyboard(const EKeyboard& device);
		__engine_decl const EMouse& GetMouse();
		__engine_decl const EKeyboard& GetKeyboard();
		__engine_decl bool KeyDown(int Key)const;
		__engine_decl bool KeyPressed(int Key)const;

	private:
		EMouse m_mouse;
		EKeyboard m_keyboard;
	};
}

