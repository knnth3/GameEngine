#include "pch.h"
#include "GS_MainMenu.h"
#include <Lime_Engine\EngineResources.h>
#include <Graphics\BrushManager.h>

using namespace Graphics;
using namespace LIME_ENGINE;
using namespace std;



bool GameStates::MainMenu::CreateDeviceIndependentResources()
{
	m_b1 = Button(100.0f, 30.0f, 100.0f, 100.0f);
	m_b2 = Button(100.0f, 30.0f, 100.0f, 140.0f);
	m_b3 = Button(100.0f, 30.0f, 100.0f, 180.0f);
	return true;
}

void GameStates::MainMenu::CreateDeviceDependentResources()
{
	//Make a new 2D brush(texture)
	std::wstring err;

	int ID = BrushManager::CreateNewImage("Assets/Textures/MainMenu/background.jpg", err);
	if (!err.empty())
		EngineResources::GetConsole()->Log(err);

	m_background.SetBrush(ID, BRUSH_TEXTURE_IMAGE);
	OnWindowResize();
}

//Gamestates control the logic for what happens every frame
//This includes what happens at the end of a gamestate
GameStates::States GameStates::MainMenu::Update()
{
	auto timer = EngineResources::GetTimer();
	auto input = EngineResources::GetInputManager();

	glm::vec2 mousePos = input->GetMouseCoords();

	bool bLMBPressed = input->LMBPressed();
	m_b1.Update(bLMBPressed);
	m_b2.Update(bLMBPressed);
	m_b3.Update(bLMBPressed);
	m_mouseCoords = "x = " + to_string(mousePos.x) + ", y = " + to_string(mousePos.y);
	//If the button is clicked, load the map editor
	if (m_b1.IsClicked())
		return States::MAP_EDITOR;
	else if (m_b2.IsClicked())
		return States::SCENE_MANAGER;
	else if (m_b3.IsClicked())
		return States::TESTING_REALM;
	else
		return States::RUNNING;
}

void GameStates::MainMenu::Close()
{
	auto graphics = EngineResources::GetGraphicsDevice();
	BrushManager::DeleteImage(m_background.GetBrush());
	graphics->Reset();
}

//Resize background on window resize
void GameStates::MainMenu::OnWindowResize()
{
	float width, height;
	EngineResources::GetGraphicsDevice()->GetWindowDimensions(width, height);
	m_background.SetPosition(0.0f, 0.0f);
	m_background.SetDimensions(width, height);
	m_mouseCoords.SetBounds(width, height);
}

//Draw all objects
void GameStates::MainMenu::Draw()
{
	auto graphics = EngineResources::GetGraphicsDevice();
	graphics->Draw(m_background);
	graphics->Draw(m_b1);
	graphics->Draw(m_b2);
	graphics->Draw(m_b3);
	graphics->Draw(m_mouseCoords);
}
