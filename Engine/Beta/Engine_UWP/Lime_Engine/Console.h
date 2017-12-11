#pragma once
#include "DllSettings.h"
#include <Graphics\Graphics.h>
#include <deque>

namespace LIME_ENGINE
{
	class Console
	{
	public:
		Console();

		//Write-to functions
		ENGINE_DLL_API void Log(std::wstring text);
		ENGINE_DLL_API void Log(std::wstring text, int brushID);

		//Display calls
		ENGINE_DLL_API void Update();
		ENGINE_DLL_API void Render();
		ENGINE_DLL_API void Switch();
		ENGINE_DLL_API void UpdateDimensions();

		//User functions
		ENGINE_DLL_API void SetTextColor(glm::vec4 color);
		ENGINE_DLL_API void SetBGColor(glm::vec4 color);
		ENGINE_DLL_API void SetToggleButton(int code);

	private:
		void Push_Back(Graphics::Text& newStr);
		void ReorderStrings();
		glm::vec3 EvaluateColor(ENGINE_COLORS color);

		int m_textBrush;
		bool m_bActive;
		float m_width;
		float m_height;
		int m_openButton;
		std::mutex m_mutex;
		Graphics::Text m_fpsText;
		Graphics::Square m_background;
		std::deque<Graphics::Text> m_strings;
	};
}

