#include "GS_SceneManager.h"
#include "DungeonMapTileset.h"

SceneManager::SceneManager()
{
	std::vector<Lime::Model::Vertex> test1;
	std::vector<std::string> test2 =
	{
		"This is a test!"
	};
	FileManager::WriteFile("testFile.txt", test1, test2);
	FileManager::LoadFile("testFile.txt", test1, test2);
}

SceneManager::SceneManager(std::unique_ptr<Scene>& loadingScene, std::unique_ptr<Scene>& activeScene)
{
	m_onSuspend = nullptr;
	m_active = nullptr;
	m_onSuspend.swap(loadingScene);
	m_active.swap(activeScene);
}

SceneManager::~SceneManager()
{
}

GameStates::States SceneManager::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
	return GameStates::States::RUNNING;
}

void SceneManager::Initialize(int windowWidth, int windowHeight)
{
}

void SceneManager::Close()
{
}

void SceneManager::OnWindowResize(int width, int height)
{
}
