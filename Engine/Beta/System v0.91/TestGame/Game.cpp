#include "Game.h"
#include <Graphics\MeshLoader.h>

using namespace Graphics;
Game::Game(uint16_t windowWidth, uint16_t windowHeight):
	App(windowWidth, windowHeight)
{
	m_character = nullptr;
	m_2dModel = 
	{
		{ { 0.0f, 0.0f, 0.0f },  { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f } },
		{ { 400.0f, 0.0f, 0.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f } },
		{ { 400.0f, 300.0f, 0.0f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f } }
	};
	m_2dModelIndex = { 0, 1 , 2 };
}

void Game::Tick()
{
	static float rotation = 0.0f;
	rotation += 0.002f;
	auto graphics = this->GetGraphicsDevice();
	auto input = this->GetInputManager();
	auto pos2d = input->GetMouseCoords();
	auto pos = input->GetMouse3DPosition();
	m_character3->SetPosition(pos.x, 0.0f, pos.z);
	//m_character->SetPosition(pos.x, 0.0f, 0.0f);
	m_character->Rotate(rotation, 0.0f, 0.0f);
	graphics->BeginScene(1.0f, 0.0f, 0.0f);
	graphics->Draw(m_character);
	graphics->Draw(m_character2);
	graphics->Draw(m_character3);
	graphics->EndScene();
}

void Game::OnInitialize()
{
	this->GetCamera()->SetRotation(glm::vec3(M_PI/2.0f, M_PI, 0.0f));
	MeshID id = MeshLoader::LoadModel(m_2dModel, m_2dModelIndex, "Testing");
	m_character = std::make_shared<Model>();
	m_character->SetDrawStyle(DrawStyle::TRIANGLE_3D);
	this->GetCamera()->AttachToModel(m_character);

	m_character2 = std::make_shared<Model>(id);
	m_character2->SetDrawStyle(DrawStyle::TRIANGLE_2D);

	m_character3 = std::make_shared<Model>();
	m_character3->SetDrawStyle(DrawStyle::TRIANGLE_3D);
	m_character3->SetPosition(20.0f, 0.0f, 0.0f);
}