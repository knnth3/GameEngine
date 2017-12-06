#pragma once
#include "DllSettings.h"
#include <Graphics\Graphics.h>
#include <deque>

namespace LIME_ENGINE
{
	class Console
	{
	public:
		Console(std::shared_ptr<Graphics::GraphicsDevice>& graphicsDevice);

		//Write-to functions
		ENGINE_DLL_API void Log(std::wstring text);
		ENGINE_DLL_API void Log(std::wstring text, std::string brushName);

		//Display calls
		ENGINE_DLL_API void Update();
		ENGINE_DLL_API void Render();
		ENGINE_DLL_API void Activate(bool value);
		ENGINE_DLL_API void Switch();
		ENGINE_DLL_API void UpdateDimensions(float width, float height);

		//User functions
		ENGINE_DLL_API void SetTextColor(glm::vec4 color);
		ENGINE_DLL_API void SetBGColor(glm::vec4 color);

	private:
		void CreateCommonBrushes();
		void ShiftOtherStrings();
		glm::vec3 EvaluateColor(ENGINE_COLORS color);

		bool m_bActive;
		float m_width;
		float m_height;
		std::mutex m_mutex;
		Graphics::Square m_background;
		std::deque<Graphics::Text> m_strings;
		std::shared_ptr<Graphics::GraphicsDevice> m_graphics;
	};
}

