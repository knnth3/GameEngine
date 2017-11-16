#pragma once
#include "DllSettings.h"
#include "Lime_String.h"
#include <deque>

namespace LIME_ENGINE
{
	class Console
	{
	public:
		Console(const uint16_t windowWidth);
		~Console();

		//Write-to functions
		ENGINE_DLL_API void Log(std::string text, float opacity = 1.0f);
		ENGINE_DLL_API void Log(std::string text, ENGINE_COLORS color, float opacity = 1.0f);
		ENGINE_DLL_API void Log(std::string text, glm::vec3 color, float opacity = 1.0f);
		ENGINE_DLL_API void Log(std::string text, float red, float green, float blue, float opacity = 1.0f);

		//Display calls
		ENGINE_DLL_API void RenderConsole(std::shared_ptr<Graphics::GraphicsDevice>& device);
		ENGINE_DLL_API void Activate(bool value);
		ENGINE_DLL_API void Switch();

		//User functions
		ENGINE_DLL_API void SetBackground(Graphics::TextureID id);
		ENGINE_DLL_API void SetOpacity(float value);
		ENGINE_DLL_API void SetColor(ENGINE_COLORS color);
		ENGINE_DLL_API void SetColor(glm::vec3 color);
		ENGINE_DLL_API void SetColor(float red, float green, float blue);

	private:
		void ShiftOtherStrings();
		glm::vec3 EvaluateColor(ENGINE_COLORS color);

		bool m_bActive;
		float m_length;
		float m_width;
		std::mutex m_mutex;
		std::shared_ptr<Graphics::Model> m_model;
		std::deque<Lime_String> m_strings;
	};
}

