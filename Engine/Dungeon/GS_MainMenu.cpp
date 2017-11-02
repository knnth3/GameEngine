#include "GS_MainMenu.h"
#include <Lime\TextureManager.h>

using namespace Lime::Model;

//Initialize Variables
GameStates::MainMenu::MainMenu(std::shared_ptr<Lime::RenderBatch>& rend)
{
	//Make an new button
	m_b1 = std::make_shared<Button>(100.0f, 100.0f, 100.0f, 30.0f);
	//Make a new texture
	auto id = Lime::TextureManager::CreateNewTexture(L"Assets/textures/MainMenuBG.dds");

	//A 2D model has 2 initializers you can choose from
	m_background = std::make_shared<Model2D>();
	m_background->SetTexture(id);
	m_background->SetColor(1.0f, 1.0f, 3.0f);
	//Add all models relevant to render batch
	rend->Add2DModel(m_background);
	rend->Add2DModel(m_b1->GetModel());
}

GameStates::MainMenu::~MainMenu()
{
}

//Gamestates control the logic for what happens every frame
//This includes what happens at the end of a gamestate
GameStates::States GameStates::MainMenu::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
	m_rad += 9.8f * time / 20.0f;
	glm::vec2 mousePos = input->GetMouseCoords();
	bool bLMBDown = input->KeyStatus(Lime::Key::Left_Mouse_Button);
	m_b1->Update(mousePos, bLMBDown);

	Draw();

	//If the button is clicked, load the map editor
	if(m_b1->IsClicked())
		return States::MAP_EDITOR;
	else
		return States::RUNNING;
}

//Do all logic here
void GameStates::MainMenu::Initialize(int windowWidth, int windowHeight)
{
	m_background->SetPosition((float)windowWidth / 2.0f, (float)windowHeight / 2.0f);
	m_background->SetLength((float)windowWidth);
	m_background->SetWidth((float)windowHeight);
}

void GameStates::MainMenu::Close()
{
}

//Resize background on window resize
void GameStates::MainMenu::OnWindowResize(int width, int height)
{
	m_background->SetPosition((float)width / 2.0f, (float)height / 2.0f);
	m_background->SetLength((float)width);
	m_background->SetWidth((float)height);
}

//Draw all objects
void GameStates::MainMenu::Draw()
{
	m_background->Draw();
	m_b1->Draw();
}
