#include "GS_MapEditor.h"
#include <Lime\ModelLoader.h>
#include "DungeonMapTileset.h"

//Macros
#define BLOCK_RADIUS 25.0f
#define BLOCK_HEIGHT METER * 0.5f

GameStates::MapEditor::MapEditor(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera)
{
	m_camera = camera;
	m_render = rend;
}

//This function can be used for pretty much anything that should be calculated every frame
//There are different type of return values it can return
//They can be found in the Gamestates::States struct located in the GameState header file
GameStates::States GameStates::MapEditor::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
	static float total = 0.0f;
	UpdateCursor(time, input);
	camRot = 9.8f / 2.0f * time;
	if (input->KeyStatus(Lime::Key::A) == true)
	{
		m_camera->Rotate(camRot, 0.0f, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::D) == true)
	{
		m_camera->Rotate(-camRot, 0.0f, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::W) == true)
	{
		m_camera->Rotate(0.0f, camRot, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::S) == true)
	{
		m_camera->Rotate(0.0f, -camRot, 0.0f);
	}
	if (input->KeyStatus(Lime::Key::Q) == true)
	{
		m_camera->Zoom(-camRot * 100.0f);
	}
	if (input->KeyStatus(Lime::Key::E) == true)
	{
		m_camera->Zoom(camRot * 100.0f);
	}
	if (input->KeyPressed(Lime::Key::P))
	{
		DMT outFile;
		outFile.AddData(m_map);
		FileManager::WriteFile("Maps/Level1.dmt", outFile);
		return SCENE_MANAGER;
	}

	Draw();

	//Running represents that the GameState is still running
	return RUNNING;
}

//All initialization of complidated logic goes in here
void GameStates::MapEditor::Initialize(int windowWidth, int windowHeight)
{
	//Load in a mesh with this function, an id is returned that's basically like a pointer to the mesh
	//The mesh loader can only open .fbx files although .obj files can be converted to .fbx
	MeshID id = Lime::Model::MeshLoader::LoadModel("Assets/Models/Cube_TextureWrap.fbx");
	MeshID modelMesh = Lime::Model::MeshLoader::LoadModel("Assets/Models/Model_Girl.fbx");

	//m_user is a 3D-Model and requires a mesh for its constructor
	m_user = std::make_shared<Lime::Model::Model3D>(modelMesh);

	//A 3D-Model has a handful of useful functions to utilize
	m_user->Scale(20.0f, 20.0f, 20.0f);

	//The camera will now rotate around this model
	m_camera->AttachToModel(m_user);

	//Cursor is also a 3D-Model and will be created the exact same as the model
	m_cursor = std::make_shared<Lime::Model::Model3D>(id);
	float radius = BLOCK_RADIUS / m_cursor->GetHeight();
	float height = BLOCK_HEIGHT / m_cursor->GetHeight();
	m_cursor->Scale(radius, height, radius);
	m_cursor->SetPosition(METER * 0.5f, METER * 0.25f, METER * 0.5f);

	//Creates a grid of 3D-Lines which is another type that the render batch can draw
	float length = METER * 5.0f;
	glm::vec4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (float x = -length; x <= length; x += METER)
	{
		glm::vec3 pos1 = { -length, 0.1f, x };
		glm::vec3 pos2 = { length, 0.1f, x };
		m_render->Add3DLine(pos1, pos2, color);
	}
	for (float x = -length; x <= length; x += METER)
	{
		glm::vec3 pos1 = { x, 0.1f, -length };
		glm::vec3 pos2 = { x, 0.1f, length };
		m_render->Add3DLine(pos1, pos2, color);
	}

	//Calls the CreateFloor helper function described in the header file
	CreateFloor(10, 10, id);

	//!Important! - every model must be passed to the render batch if it's gong to be drawn to the screen
	m_render->Add3DModel(m_user);
	m_render->Add3DModel(m_cursor);
}

//Nothing here requires a manual close
void GameStates::MapEditor::Close()
{
}

//Completely unessesary right now. Can be deleted as described in GameStates header file
void GameStates::MapEditor::OnWindowResize(int width, int height)
{
}

void GameStates::MapEditor::Draw()
{
	//Call all the objects that want to be drawn ->Draw() function provided in every Model
	m_user->Draw();
	m_cursor->Draw();

	for (auto x : m_map)
	{
		x->Draw();
	}
}

void GameStates::MapEditor::CreateFloor(int length, int width, MeshID id)
{
	//Textures can be created with the TextureManager, if no texture is provided to a model, it will default to a white color
	Lime::TextureID texture = Lime::TextureManager::CreateNewTexture(L"Assets/textures/image1.dds");

	//Logic

	//Offset for each block from (0,0,0)
	float alignPos = (METER * 0.5f);
	for (int x = 0; x < length; x++)
	{
		for (int y = 0; y < width; y++)
		{
			//Set the settings for every element in the 2d vector
			float offsetX = float(length / 2) * METER;
			float offsety = float(width / 2) * METER;
			float posX = float(x) * METER;
			float posY = float(y) * METER;

			//Creates a floor tile and adds it to the map
			auto tile = std::make_shared<Lime::Model::Model3D>(id);
			tile->Scale(10.0f, 2.5f, 10.0f);
			tile->SetPosition(alignPos + posX - offsetX, METER * -0.125f, alignPos + posY - offsety);
			tile->SetTexture(texture);

			//Add to render batch
			m_render->Add3DModel(tile);
			m_map.push_back(tile);
		}
	}
}

void GameStates::MapEditor::UpdateCursor(float time, std::shared_ptr<Lime::InputManager>& input)
{
	//Static means the variable will stay relevant even after the end of the function
	static float alignPos = BLOCK_RADIUS;
	static float slices = BLOCK_RADIUS * 2.0f;
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
	m_cursor->SetPosition(posX + alignPos, METER * 0.5f, posY + alignPos);

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

//Copy's the passed model and creates a new one from it
void GameStates::MapEditor::AddNewBlock(std::shared_ptr<Lime::Model::Model3D>& model)
{
	auto newBlock = std::make_shared<Lime::Model::Model3D>(model->GetMesh());
	newBlock->SetColor(model->GetColor());
	auto scale = model->GetScale();
	newBlock->Scale(scale);
	newBlock->SetPosition(model->GetPosition());

	//Needs to be added to the render batch
	m_render->Add3DModel(newBlock);

	//Adds to the new block vector described in the header file
	m_map.push_back(newBlock);
}
