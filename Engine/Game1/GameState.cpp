#include "GameState.h"
#include "Engine\MeshLoader.h"

using namespace Engine;

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::Initialize()
{
	WindowResources::GetGraphics()->GetTextureLibrary()->CreateTexture("PLANE_BG", "Assets/textures/grid_tile.png");
	int id = MeshLoader::CreatePlane(100, 100, 100, 100);
	m_plane.SetMesh(id);
	m_plane.SetTexture("PLANE_BG");

	//Create main character
	CharacterInfo info;
	info.Name = "Alfie";
	info.Speed = 0.25f;
	m_user = std::make_unique<Character>(info, "Assets/Models/body_plate_silver_epic.sef", "Assets/Models/head1.sef");
	m_user2 = std::make_unique<Character>(info, "Assets/Models/body_plate_silver_epic.sef", "Assets/Models/head1.sef");

	WindowResources::GetGraphics()->GetCamera()->AttachToModel(m_user->GetModel());
}

void GameState::Update(double elapsed)
{
	auto mouse = WindowResources::GetInput()->GetMouse();
	if (mouse->ButtonPressed(0))
	{
		auto mousePos = mouse->Get3DPosition();
		m_user->SetDestination(mousePos);
	}

	m_user->Update(elapsed);
	m_user2->Update(elapsed);
}

void GameState::Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)
{
	graphics->Draw(m_plane);
	graphics->Draw(m_user->GetModel());
	graphics->Draw(m_user2->GetModel());
}

void GameState::Close()
{
	WindowResources::GetGraphics()->GetTextureLibrary()->DeleteTexture("PLANE_BG");
}
