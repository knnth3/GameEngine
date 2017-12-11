#pragma once
#include "GameState.h"
#include "Button.h"
#include <Graphics\Shape_Square.h>
#include <Graphics\Text.h>

//Main menu implementation
//Inherits from GameState
//Contains a basic implementation that will be replaced in the future

namespace GameStates
{
	class MainMenu :
		public GameState
	{
	public:

		//State Changes
		virtual bool CreateDeviceIndependentResources()override;
		virtual void CreateDeviceDependentResources()override;
		virtual States Update()override;
		virtual void Draw()override;

		virtual void Close()override;
		virtual void OnWindowResize()override;

	private:
		Graphics::Square m_background;
		Graphics::Text m_mouseCoords;
		Button m_b1;
		Button m_b2;
	};

}
