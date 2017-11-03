#include "Scene_Level1.h"
#include "DungeonMapTileset.h"
#include <Lime\ModelLoader.h>
using namespace Lime::Model;
using namespace std;


Scenes::Scene_Level1::Scene_Level1(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera):
	Scene(rend, camera)
{
	DMT inFile;
	FileManager::LoadFile("Maps/Level1.dmt", inFile);

	for (auto x : inFile.objects)
	{
		m_map.emplace_back(make_shared<Model3D>(MeshLoader::LoadModel(x.vertices, x.indices)));
		auto & model = m_map.at(m_map.size() - 1);
		model->SetPosition(x.position);
		model->Scale(x.scale);
		model->SetColor(1.0f, 0.0f, 0.0f);
		rend->Add3DModel(model);
	}
	if (m_map.size() > 0)
	{
		camera->AttachToModel(m_map[0]);
	}
}

Scenes::Scene_Level1::~Scene_Level1()
{
}

void Scenes::Scene_Level1::Initialize()
{
}

void Scenes::Scene_Level1::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
	if (input->KeyPressed(Lime::Key::ESC))
	{
		this->SetCurrentState(RunState::MAIN_MENU);
	}

	for (auto x : m_map)
		x->Draw();
}
