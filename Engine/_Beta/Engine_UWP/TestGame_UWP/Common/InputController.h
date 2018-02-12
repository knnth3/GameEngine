#pragma once
#include <Lime_Engine\InputManager.h>

// Uncomment the next line to print debug tracing information.
// #define MOVELOOKCONTROLLER_TRACE 1

namespace InputConstants
{
	static const float RotationGain = 0.008f;       // Sensitivity adjustment for look controller.
	static const float MovementGain = 2.0f;         // Sensitivity adjustment for move controller.
};

ref class InputController
{
internal:
	InputController(_In_ Windows::UI::Core::CoreWindow^ window);
	void AttatchCamera(_Inout_ std::shared_ptr<Graphics::Camera>& camera);
	std::shared_ptr<LIME_ENGINE::InputManager> GetManager();
	void Update();
	bool  Active();
	void  Active(_In_ bool active);
	void ShowCursor();
	void HideCursor();

private:
	void InitWindow(_In_ Windows::UI::Core::CoreWindow^ window);
	void UpdatePollingDevices();
	bool PollingFireInUse() { return m_gamepadTriggerInUse; }

	void OnPointerPressed(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::PointerEventArgs^ args
	);
	void OnPointerMoved(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::PointerEventArgs^ args
	);
	void OnPointerReleased(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::PointerEventArgs^ args
	);
	void OnPointerExited(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::PointerEventArgs^ args
	);
	void OnKeyDown(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::KeyEventArgs^ args
	);
	void OnKeyUp(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::KeyEventArgs^ args
	);
	void OnGamepadAdded(
		_In_ Platform::Object^ sender,
		_In_ Windows::Gaming::Input::Gamepad^ gamepad
	);
	void OnGamepadRemoved(
		_In_ Platform::Object^ sender,
		_In_ Windows::Gaming::Input::Gamepad^ gamepad
	);

#ifdef MOVELOOKCONTROLLER_TRACE
	void DebugTrace(const wchar_t *format, ...);
#endif

private:

	bool m_bActive;
	std::shared_ptr<LIME_ENGINE::InputManager> m_inputManager;
	// Game controller related members.
	Windows::Gaming::Input::Gamepad^    m_activeGamepad;
	std::atomic<bool>                   m_gamepadsChanged;
	bool                                m_gamepadStartButtonInUse;
	bool                                m_gamepadTriggerInUse;
};
