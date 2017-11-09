#include "Game.h"

Game::Game(uint16_t windowWidth, uint16_t windowHeight):
	App(windowWidth, windowHeight)
{
	m_character = nullptr;
}

void Game::Tick()
{
	static float rotation = 0.0f;
	rotation += 0.002f;
	m_character->Rotate(0.0f, 0.0f, rotation);
	auto graphics = this->GetGraphicsDevice();
	graphics->BeginScene(1.0f, 0.0f, 0.0f);
	graphics->Draw(m_character);
	graphics->EndScene();
}

void Game::OnInitialize()
{
	m_character = std::make_shared<Model::Model3D>();
	this->GetCamera()->AttachToModel(m_character);
}