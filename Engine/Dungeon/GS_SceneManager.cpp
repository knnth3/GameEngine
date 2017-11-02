#include "GS_SceneManager.h"
#include "DungeonMapTileset.h"
#include <Lime\ModelLoader.h>

SceneManager::SceneManager(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera)
{
	DMT inFile;
	FileManager::LoadFile("Maps/Level1.dmt", inFile);

	for (auto x : inFile.objects)
	{
		m_map.emplace_back(std::make_shared<Lime::Model::Model3D>(Lime::Model::MeshLoader::LoadModel(x.vertices, x.indices)));
		auto & model = m_map.at(m_map.size() - 1);
		model->SetPosition(x.position);
		model->Scale(x.scale);
		rend->Add3DModel(model);
	}
	if (m_map.size() > 0)
	{
		camera->AttachToModel(m_map[0]);
	}

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
	for (auto x : m_map)
		x->Draw();


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
