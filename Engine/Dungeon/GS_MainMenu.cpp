#include "GS_MainMenu.h"
#include <Lime\TextureManager.h>

using namespace Lime::Model;

GameStates::MainMenu::MainMenu(std::shared_ptr<Lime::DX11Graphics>& rend)
{
	m_b1 = std::make_shared<Button>(100.0f, 100.0f, 100.0f, 30.0f);
	auto id = Lime::TextureManager::CreateNewTexture(L"Assets/textures/MainMenuBG.dds");
	m_test = std::make_shared<Model2D>();
	m_test->SetTexture(id);
	m_test->SetColor(1.0f, 1.0f, 3.0f);
	//Add all models relevant to the scene
	rend->Add2DModel(m_test);
	rend->Add2DModel(m_b1->GetModel());
}

GameStates::MainMenu::~MainMenu()
{
}

GameStates::States GameStates::MainMenu::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
	m_rad += 9.8f * time / 20.0f;
	glm::vec2 mousePos = input->GetMouseCoords();
	bool bLMBDown = input->KeyStatus(Lime::Key::Left_Mouse_Button);
	m_b1->Update(mousePos, bLMBDown);

	Draw();

	if(m_b1->IsClicked())
		return States::MAP_EDITOR;
	else
		return States::RUNNING;
}

void GameStates::MainMenu::Initialize(int windowWidth, int windowHeight)
{
	m_test->SetPosition((float)windowWidth / 2.0f, (float)windowHeight / 2.0f);
	m_test->SetLength((float)windowWidth);
	m_test->SetWidth((float)windowHeight);
}

void GameStates::MainMenu::Close()
{
}

void GameStates::MainMenu::OnWindowResize(int width, int height)
{
	m_test->SetPosition((float)width / 2.0f, (float)height / 2.0f);
	m_test->SetLength((float)width);
	m_test->SetWidth((float)height);
}

void GameStates::MainMenu::Draw()
{
	m_test->Draw();
	m_b1->Draw();
}
