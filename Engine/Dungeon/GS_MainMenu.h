#pragma once
#include "GameState.h"
#include "Button.h"
#include <Lime\DX11Graphics.h>
#include <Lime\Model2D.h>

//Main menu implementation
//Inherits from GameState
//Contains a basic implementation that will be replaced in the future

namespace GameStates
{
	class MainMenu :
		public GameState
	{
	public:
		MainMenu(std::shared_ptr<Lime::RenderBatch>& rend);
		~MainMenu();

		//Function called every frame
		virtual States Update(float time, std::shared_ptr<Lime::InputManager>& input) override;

		//State Changes
		virtual void Initialize(int windowWidth, int windowHeight) override;
		virtual void Close() override;
		virtual void OnWindowResize(int width, int height) override;



		void Draw();

	private:
		std::shared_ptr<Lime::Model::Model2D> m_background;
		std::shared_ptr<Button> m_b1;

		float m_rad = 0.0f;

	};

}
