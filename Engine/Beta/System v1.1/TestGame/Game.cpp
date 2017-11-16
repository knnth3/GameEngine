#include "Game.h"
#include <Graphics\MeshLoader.h>
#include <Graphics\DX11TextureManager.h>

using namespace Graphics;
Game::Game(uint16_t windowWidth, uint16_t windowHeight):
	App(windowWidth, windowHeight)
{
	//Create a custom mesh for the 2D object
	m_2dModel = 
	{
		{ { 0.0f, 600.0f, 0.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, 0.0f } },
		{ { 0.0f, 0.0f, 0.0f },  { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f } },
		{ { 800.0f, 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f } },

		{ { 800.0f, 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f } },
		{ { 800.0f, 600.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, 0.0f } },
		{ { 0.0f, 600.0f, 0.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, 0.0f } },
		
	};
	m_2dModelIndex = { 0, 1, 2, 3, 4, 5 };
}

void Game::Tick()
{
	//Set nessesary variables
	static float rotation = 0.0f;
	rotation += 0.002f;
	auto graphics = this->GetGraphicsDevice();
	auto input = this->GetInputManager();
	auto pos = input->GetMouse3DPosition();

	//Update
	m_character3->SetPosition(pos.x, 0.0f, pos.z);
	m_character->Rotate(rotation, 0.0f, 0.0f);

	//Draw
	graphics->BeginScene(1.0f, 0.0f, 0.0f);
	graphics->Draw(m_character2);
	graphics->Draw(m_character);
	graphics->Draw(m_character3);
	graphics->EndScene();
}

void Game::OnInitialize()
{
	//Reverse the draw order
	this->GetGraphicsDevice()->DrawBackToFront(false);

	//Grab nessesary data before object creation
	this->GetCamera()->SetRotation(glm::vec3(M_PI/2.0f, M_PI, 0.0f));
	MeshID id = MeshLoader::LoadModel(m_2dModel, m_2dModelIndex, "Custom_Mesh");
	TextureID tex = DX11TextureManager::CreateNewTexture("EngineAssets/textures/test_image.png");

	//Create main character and attatch camera to it
	m_character = std::make_shared<Model>();
	m_character->SetDrawStyle(DrawStyle::TRIANGLE_3D);
	this->GetCamera()->AttachToModel(m_character);

	//Create next object as 2D with no cull
	m_character2 = std::make_shared<Model>(id);
	m_character2->SetTexture(tex);
	m_character2->SetDrawStyle(DrawStyle::TRIANGLE_2D);
	m_character2->SetRSS(RSS_NO_CULL);
	m_character2->Set2DuvInfo(0.5f, 0.5f, 0.5f, 0.5f);

	//Create last object as 3D with Wireframe
	m_character3 = std::make_shared<Model>();
	m_character3->SetDrawStyle(DrawStyle::TRIANGLE_3D);
	m_character3->SetPosition(20.0f, 0.0f, 0.0f);
	m_character3->SetRSS(RSS_WIREFRAME);
}