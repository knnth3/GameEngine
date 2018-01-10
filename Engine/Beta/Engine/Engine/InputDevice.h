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
		__engine_decl virtual bool ButtonPressed(int key)final;
		__engine_decl virtual bool ButtonDown(int key)final;

	protected:
		friend class GraphicsWindow;
		__engine_decl virtual void SetButtonStatus(int key, bool value)final;
		__engine_decl virtual int ConvertKeyToButton(int key);

	private:
		std::mutex m_lock;
		std::unordered_map<int, bool> m_KeysDown;
		std::unordered_map<int, bool> m_KeysPressed;
	};

}
