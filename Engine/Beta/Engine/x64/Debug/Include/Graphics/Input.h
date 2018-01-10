#pragma once
#include "MouseInput.h"


namespace Graphics
{
	class Input
	{
	public:
		GRAPHICS_DLL_API Input();
		GRAPHICS_DLL_API void AttatchKeyboard(const std::shared_ptr<InputDevice>& device);
		GRAPHICS_DLL_API const std::shared_ptr<MouseInput>& GetMouse();
		GRAPHICS_DLL_API const std::shared_ptr<InputDevice>& GetKeyboard();
		GRAPHICS_DLL_API bool KeyDown(int Key)const;
		GRAPHICS_DLL_API bool KeyPressed(int Key)const;

	private:
		std::shared_ptr<MouseInput> m_mouse;
		std::shared_ptr<InputDevice> m_keyboard;
	};
}

