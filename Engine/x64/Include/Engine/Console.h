#pragma once
#include "DllSettings.h"
#include "GraphicsDevice.h"
#include <queue>

namespace Engine
{
	class Console
	{
	public:
		__engine_decl Console(int maxLogs, float width = 0.0f, float height = 0.0f);
		__engine_decl ~Console();
		__engine_decl void Log(const std::string& message);
		__engine_decl Square& GetBackground();
		__engine_decl const Text& At(int index)const;
		__engine_decl int GetSize()const;
		__engine_decl float GetTextHeight()const;
		__engine_decl void SetDimensions(float width, float height);
		__engine_decl void SetTextHeight(float value);
	private:

		int m_textureID;
		float m_width;
		float m_height;
		float m_textHeight;
		int m_currentIndex;
		const int m_maxLogs;
		Text** m_messages;
		Square m_background;
	};
}
