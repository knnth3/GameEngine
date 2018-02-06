#pragma once
#include "GraphicsDevice.h"
#include "Timer.h"
#include "WindowResources.h"
#include <atomic>

namespace Engine
{
	/*This is an abstract class implementation that requires
	certain functions to be provided by child class.
	These include: 
	-Update(void)
	-Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)
	-Resume(void)
	-Suspend(void)
	Resume and Suspend functions act as start-up/shut-down functions.
	Do not expect to have these call only in those scenarios
	as they may be called whenever the system seems fit.
	All important interfaces can be obtained through the WindowResources static class.
	Ex: Input and Timer interfaces*/
	class GraphicsWindow
	{
	public:
		__engine_decl GraphicsWindow(const std::string WindowTitle, float width = 800.0f, float height = 600.0f, bool fullscreen = false);
		__engine_decl GraphicsWindow(const std::wstring WindowTitle, float width = 800.0f, float height = 600.0f, bool fullscreen = false);
		__engine_decl ~GraphicsWindow();
		__engine_decl virtual bool Initialize()final;
		__engine_decl virtual int Run()final;
		__engine_decl virtual std::string GetLastError()final;
		__engine_decl virtual LRESULT WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	protected:

		__engine_decl void SetWindowWidth(float width);
		__engine_decl void SetWindowHeight(float height);
		__engine_decl void SetDisplaySize(DisplaySize size);
		__engine_decl void ToggleFullscreen();

		//Required
		virtual void Update() = 0;
		virtual void Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics) = 0;
		virtual void Resume() = 0;
		virtual void Suspend() = 0;

	private:
		bool ProcessApp();
		bool InitGraphics();
		void ShutdownGraphics();
		void ResizeWindowBySettings(bool move = false, glm::vec2 pos = glm::vec2());

		HWND m_hwnd;
		HINSTANCE m_hInstance;
		bool m_bInitialized;
		bool m_bFullscreen;
		std::atomic_bool m_bResize;
		std::atomic_bool m_brunning;
		DisplaySize m_size;
		std::string m_lastError;
		std::wstring m_windowTitle;
		std::shared_ptr<GraphicsDevice> m_graphics;
		std::shared_ptr<Timer> m_timer;
		std::shared_ptr<Input> m_input;
	};

	LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
}

