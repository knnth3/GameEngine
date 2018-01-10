#pragma once
#include "DllSettings.h"
#include <unordered_map>
#include <mutex>

namespace Graphics
{
	//ThreadSafe interface to handle user input
	class InputDevice
	{
	public:
		GRAPHICS_DLL_API virtual bool ButtonPressed(int key)final;
		GRAPHICS_DLL_API virtual bool ButtonDown(int key)final;

	protected:
		friend class GraphicsWindow;
		GRAPHICS_DLL_API virtual void SetButtonStatus(int key, bool value)final;
		GRAPHICS_DLL_API virtual int ConvertKeyToButton(int key);

	private:
		std::mutex m_lock;
		std::unordered_map<int, bool> m_KeysDown;
		std::unordered_map<int, bool> m_KeysPressed;
	};

}
