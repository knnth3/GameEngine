#pragma once
#include "Graphics.h"
#include "Timer.h"
#include "WindowResources.h"
#include <atomic>

namespace Graphics
{
	/*This is an abstract class implementation that requires
	certain functions to be provided by child class.
	These include: 
	-Update(void)
	-Render(const std::shared_ptr<Graphics::GraphicsDevice>& graphics)
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
		__engine_decl GraphicsWindow(const std::string WindowTitle);
		__engine_decl GraphicsWindow(const std::wstring WindowTitle);
		__engine_decl ~GraphicsWindow();
		__engine_decl virtual bool Initialize()final;
		__engine_decl virtual int Run()final;
		__engine_decl virtual std::string GetLastError()final;
		__engine_decl virtual LRESULT WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	protected:

		//Required
		virtual void Update() = 0;
		virtual void Render(const std::shared_ptr<Graphics::GraphicsDevice>& graphics) = 0;
		virtual void Resume() = 0;
		virtual void Suspend() = 0;

	private:
		bool ProcessApp();
		bool InitGraphics();
		void ShutdownGraphics();

		HWND m_hwnd;
		HINSTANCE m_hInstance;
		bool m_bInitialized;
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

