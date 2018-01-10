#include "InputDevice.h"


using namespace Graphics;

bool Graphics::InputDevice::ButtonPressed(int key)
{
	key = ConvertKeyToButton(key);
	std::lock_guard<std::mutex> lock(m_lock);
	auto found = m_KeysPressed.find(key);
	if (found != m_KeysPressed.end())
	{
		bool val = found->second;
		found->second = false;
		return val;
	}
	return false;
}

bool Graphics::InputDevice::ButtonDown(int key)
{
	key = ConvertKeyToButton(key);
	std::lock_guard<std::mutex> lock(m_lock);
	auto found = m_KeysPressed.find(key);
	if (found != m_KeysPressed.end())
		return found->second;

	return false;
}

void Graphics::InputDevice::SetButtonStatus(int key, bool value)
{
	key = ConvertKeyToButton(key);
	auto found = m_KeysDown.find(key);
	if (found != m_KeysDown.end())
	{
		if (found->second != value)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			if (found->second == false)
				m_KeysPressed[key] = value;
			found->second = value;
			return;
		}
	}
	else if (value == true)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_KeysDown.emplace(key, value);
		m_KeysPressed.emplace(key, value);
		return;
	}
}

int Graphics::InputDevice::ConvertKeyToButton(int key)
{
	return key;
}
