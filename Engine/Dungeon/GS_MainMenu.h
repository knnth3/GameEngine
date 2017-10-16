#pragma once
#include "GameState.h"
#include "Button.h"
#include <Lime\DX11Graphics.h>
#include <Lime\Model2D.h>

namespace GameStates
{
	class MainMenu :
		public GameState
	{
	public:
		MainMenu(std::unique_ptr<Lime::DX11Graphics>& rend);
		~MainMenu();

		//Function called every frame
		virtual int Update(float time, std::unique_ptr<Lime::InputManager>& input) override;

		//State Changes
		virtual void Initialize(int windowWidth, int windowHeight) override;
		virtual void Close() override;
		virtual void OnWindowResize(int width, int height) override;



		void Draw();

	private:
		std::shared_ptr<Lime::Model::Model2D> m_test;
		std::shared_ptr<Lime::Model::Model2D> m_test2;
		std::shared_ptr<Button> m_b1;

		float m_rad = 0.0f;

	};

}
