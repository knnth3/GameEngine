#include "Scene_Level1.h"
#include "DungeonMapTileset.h"
#include <Lime\ModelLoader.h>
using namespace Lime::Model;
using namespace std;


Scenes::Scene_Level1::Scene_Level1(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera):
	Scene(rend, camera)
{
}

Scenes::Scene_Level1::~Scene_Level1()
{
}

void Scenes::Scene_Level1::Initialize()
{
	//Load File
	DMT inFile;
	FileManager::LoadFile("Maps/Level1.dmt", inFile);
	auto render = this->GetRender();
	auto camera = this->GetCamera();

	m_map.clear();
	render->Reset();
	MeshLoader::Clear();

	for (auto x : inFile.objects)
	{
		m_map.emplace_back(make_shared<Model3D>(MeshLoader::LoadModel(x.vertices, x.indices)));
		auto & model = m_map.at(m_map.size() - 1);
		model->SetPosition(x.position);
		model->Scale(x.scale);
		model->SetColor(0.7f, 0.7f, 0.9f);
		render->Add3DModel(model);
	}

	//Character Model Change (Default is cube)
	//MeshID id = MeshLoader::LoadModel("");
	//this->GetCharacter()->SetModel(id, render, camera);
}

void Scenes::Scene_Level1::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
	ProcessInput(time, input);
	this->GetCharacter()->Update(time);
	Draw();
}

void Scenes::Scene_Level1::Draw()
{
	for (auto x : m_map)
		x->Draw();

	this->GetCharacter()->Draw();
}

void Scenes::Scene_Level1::ProcessInput(float time, std::shared_ptr<Lime::InputManager>& input)
{
	float camRot = 9.8f / 2.0f * time;
	auto camera = this->GetCamera();
	auto character = this->GetCharacter();
	if (input->KeyPressed(Lime::Key::Left_Mouse_Button) == true)
	{
		glm::vec3 mousePos = input->GetMouse3DPosition();
		cout <<"Mouse Position: ("<< mousePos.x << ", " << mousePos.y << ", " << mousePos.z << ")" << endl;
		character->SetDestination(mousePos);
	}
	if (input->KeyStatus(Lime::Key::A) == true)
	{
		camera->Rotate(camRot, 0.0f, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::D) == true)
	{
		camera->Rotate(-camRot, 0.0f, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::W) == true)
	{
		camera->Rotate(0.0f, camRot, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::S) == true)
	{
		camera->Rotate(0.0f, -camRot, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::Q) == true)
	{
		camera->Zoom(-camRot * 100.0f);
	}
	if (input->KeyStatus(Lime::Key::E) == true)
	{
		camera->Zoom(camRot * 100.0f);
	}
	if (input->KeyPressed(Lime::Key::ESC))
	{
		this->SetCurrentState(RunState::MAIN_MENU);
	}

}
