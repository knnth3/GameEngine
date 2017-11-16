#include "Game.h"
#include <Graphics\MeshLoader.h>
#include <Graphics\DX11TextureManager.h>

using namespace Graphics;
Game::Game(uint16_t windowWidth, uint16_t windowHeight):
	App(windowWidth, windowHeight)
{
}

void Game::Tick()
{
	//Set nessesary variables
	static float rotation = 0.0f;
	rotation += 0.002f;
	auto graphics = this->GetGraphicsDevice();
	m_character->Rotate(rotation, 0.0f, 0.0f);

	//Draw
	graphics->BeginScene(1.0f, 0.0f, 0.0f);
	graphics->Draw(m_character);
	graphics->EndScene();
}

void Game::OnInitialize()
{
	//Reverse the draw order
	this->GetGraphicsDevice()->DrawBackToFront(false);

	//Grab nessesary data before object creation
	this->GetCamera()->SetRotation(glm::vec3(M_PI/2.0f, M_PI, 0.0f));

	//Create main character and attatch camera to it
	m_character = std::make_shared<Model>();
	m_character->SetDrawStyle(DrawStyle::TRIANGLE_3D);
	this->GetCamera()->AttachToModel(m_character);
}