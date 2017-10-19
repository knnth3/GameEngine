#include "GS_MapEditor.h"
#include <Lime\ModelLoader.h>

#define Meter 100.0f


GameStates::MapEditor::MapEditor(std::shared_ptr<Lime::DX11Graphics>& rend, std::shared_ptr<Lime::Camera>& camera)
{
	m_camera = camera;
	m_render = rend;
	auto id = Lime::Model::MeshLoader::LoadModel("Assets/Models/Cube_TextureWrap.fbx");
	auto modelMesh = Lime::Model::MeshLoader::LoadModel("Assets/Models/Model_Girl.fbx");
	m_user = std::make_shared<Lime::Model::Model3D>(modelMesh);
	m_user->Scale(20.0f, 20.0f, 20.0f);
	m_camera->AttachToModel(m_user);

	m_cursor = std::make_shared<Lime::Model::Model3D>(id);
	m_cursor->Scale(5.0f, 10.0f, 5.0f);
	m_cursor->SetPosition(Meter * 0.5f, Meter * 0.25f, Meter * 0.5f);
	rend->Add3DModel(m_user);
	CreateFloor(10, 10, id, rend);


	float length = Meter * 5.0f;
	glm::vec4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (float x = -length; x <= length; x += Meter)
	{
		glm::vec3 pos1 = { -length, 0.1f, x };
		glm::vec3 pos2 = { length, 0.1f, x };
		rend->Add3DLine(pos1, pos2, color);
	}
	for (float x = -length; x <= length; x += Meter)
	{
		glm::vec3 pos1 = { x, 0.1f, -length };
		glm::vec3 pos2 = { x, 0.1f, length };
		rend->Add3DLine(pos1, pos2, color);
	}
	rend->Add3DModel(m_cursor);
}

GameStates::States GameStates::MapEditor::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
	static float total = 0.0f;
	UpdateCursor(time, input);
	camRot = 9.8f / 2.0f * time;
	if (input->KeyStatus(Lime::Key::A) == true) //A
	{
		m_camera->Rotate(camRot, 0.0f, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::D) == true) //D
	{
		m_camera->Rotate(-camRot, 0.0f, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::W) == true) //W
	{
		m_camera->Rotate(0.0f, camRot, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::S) == true) //S
	{
		m_camera->Rotate(0.0f, -camRot, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::Q) == true) //Q
	{
		//total += -camRot;
		//m_user->Scale(total, total, total);
		m_camera->Zoom(-camRot * 100.0f);
	}
	if (input->KeyStatus(Lime::Key::E) == true) //E
	{
		//total += camRot;
		//m_user->Scale(total, total, total);
		m_camera->Zoom(camRot * 100.0f);
	}

	Draw();
	return RUNNING;
}

void GameStates::MapEditor::Initialize(int windowWidth, int windowHeight)
{
}

void GameStates::MapEditor::Close()
{
}

void GameStates::MapEditor::OnWindowResize(int width, int height)
{
}

void GameStates::MapEditor::Draw()
{
	m_user->Draw();
	m_cursor->Draw();

	for (auto x : m_floor)
	{
		for (auto y : x)
		{
			y->Draw();
		}
	}
	for (auto x : m_newBlocks)
	{
		x->Draw();
	}
}

void GameStates::MapEditor::CreateFloor(int length, int width, Lime::TextureID id, std::shared_ptr<Lime::DX11Graphics>& rend)
{
	auto texture = Lime::TextureManager::CreateNewTexture(L"Assets/textures/image1.dds");

	float alignPos = (Meter * 0.5f);
	m_floor.resize(length);
	for (int x = 0; x < length; x++)
	{
		m_floor[x].resize(width, nullptr);
		for (int y = 0; y < width; y++)
		{
			float offsetX = float(length / 2) * Meter;
			float offsety = float(width / 2) * Meter;
			float posX = float(x) * Meter;
			float posY = float(y) * Meter;
			m_floor[x][y] = std::make_shared<Lime::Model::Model3D>(id);
			m_floor[x][y]->Scale(10.0f, 2.5f, 10.0f);
			m_floor[x][y]->SetPosition(alignPos + posX - offsetX, Meter * -0.125f, alignPos + posY - offsety);
			m_floor[x][y]->SetTexture(texture);
			rend->Add3DModel(m_floor[x][y]);
		}
	}
}

void GameStates::MapEditor::UpdateCursor(float time, std::shared_ptr<Lime::InputManager>& input)
{
	static float alignPos = (Meter * 0.25f);
	static float slices = Meter * 0.5f;
	static float color = 0.0f;
	float posX;
	float posY;

	//Position calculation
	glm::vec3 mousePos = input->GetMouse3DPosition();
	if (mousePos.x >= 0.0f)
	{
		posX = truncf(mousePos.x / slices) * slices;
	}
	else
	{
		float divs = truncf(mousePos.x / slices) - 1;
		posX = divs * slices;
	}

	if (mousePos.z >= 0.0f)
	{
		posY = truncf(mousePos.z / slices) * slices;
	}
	else
	{
		float divs = truncf(mousePos.z / slices) - 1;
		posY = divs * slices;
	}
	m_cursor->SetPosition(posX + alignPos, Meter * 0.5f, posY + alignPos);

	//Color change
	static float addFactor = 0.0f;
	addFactor += time;
	color = cosf(addFactor);

	float alpha = color;
	if (alpha < 0.7f)
		alpha = 0.7f;
	m_cursor->SetColor(color, color, 1.0f, alpha);


	//Check for mouse button click
	static bool bIsPressed = false;
	bool status = input->KeyStatus(Lime::Key::Left_Mouse_Button);
	if (status)
	{
		bIsPressed = true;
	}
	else if (bIsPressed)
	{
		//Temporary: Adds new block as cursor copy;
		bIsPressed = false;
		AddNewBlock(m_cursor);
	}


}

void GameStates::MapEditor::AddNewBlock(std::shared_ptr<Lime::Model::Model3D>& model)
{
	auto newBlock = std::make_shared<Lime::Model::Model3D>(model->GetMesh());
	newBlock->SetColor(model->GetColor());
	auto scale = model->GetScale();
	auto scale2 = glm::vec3(5.0f, 10.0f, 5.0f);
	newBlock->Scale(scale2);
	newBlock->SetPosition(model->GetPosition());
	m_newBlocks.push_back(newBlock);
	m_render->Add3DModel(newBlock);
}
