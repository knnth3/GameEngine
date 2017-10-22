#include "GS_MapEditor.h"
#include <Lime\ModelLoader.h>

//These are macros and can replace any "hard-coding" with a variable name that will be replaced
//-by the c++ compliler in compile time
//More information can be found online
//These shouldnt be used unless you have a good grasp on the subject
//If so, make them capital only to distinguish them easily
#define BLOCK_RADIUS 12.5f
#define BLOCK_HEIGHT METER * 0.5f

GameStates::MapEditor::MapEditor(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera)
{
	//All objects described in the header file is zero-ed or set in here
	//Ptrs to the active camera and render batch is obtained from the constructor
	m_camera = camera;
	m_render = rend;
}

//This function can be used for pretty much anything that should be calculated every frame
//Here I poll for the user input and update the cursor
//There are different type of return values it can return
//They can be found in the Gamestates::States struct located in the GameState header file
//If intellisense is available, just type GameStates::States:: and the options will appear
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

	Draw();

	//Running represents that the GameState is still running
	return RUNNING;
}

//All initialization of complidated logic goes in here
void GameStates::MapEditor::Initialize(int windowWidth, int windowHeight)
{
	//New mesh IDs are made for the objects in the scene
	//The mesh loader can only open .fbx files although .obj files can be converted to .fbx
	//Later, a .obj reader will be implemented
	//****!The auto variable type is new to c++. It can be used to replace any type name with someting that is easy to type*****
	//-Auto in this case represents a MeshID. You can definitely write MeshID instead of auto (and is encouraged) but sometimes names are
	//-too long to type so auto is preffered
	//-The downside is that auto makes it harder to follow code
	auto id = Lime::Model::MeshLoader::LoadModel("Assets/Models/Cube_TextureWrap.fbx");
	MeshID modelMesh = Lime::Model::MeshLoader::LoadModel("Assets/Models/Model_Girl.fbx");

	//m_user is a 3D-Model and requires a mesh for its constructor
	//Since this will represent the character in this scene, use the camera's ->AttachToModel(model)
	//-This tells the camera to follow/rotate around the selected model
	m_user = std::make_shared<Lime::Model::Model3D>(modelMesh);
	//A 3D-Model has a handful of useful functions to utilize
	//Scale is one of many. Remember a scale of 1 is default
	m_user->Scale(20.0f, 20.0f, 20.0f);
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
	//-It cannot draw anything it doesn't know about
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

//Call all the objects that want to be drawn ->Draw() function provided in every Model
//--Call only if you want to draw the object this frame
void GameStates::MapEditor::Draw()
{
	m_user->Draw();
	m_cursor->Draw();

	//Uses iterator ptrs (Part of the STL library) to go through every object in the vector
	//More information about this can be found online and by watching this video
	//https://www.youtube.com/watch?v=h5aFJJp1Stw
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

//Creates the floor (the objects that have the 4 as a texture)
void GameStates::MapEditor::CreateFloor(int length, int width, MeshID id)
{
	//Textures can be created with the TextureManager
	//Can use auto here if desired
	//-an ID with -1 is default for a white texture and does not need to be set on a model if desired
	Lime::TextureID texture = Lime::TextureManager::CreateNewTexture(L"Assets/textures/image1.dds");

	//Logic

	//Offset for each block from the center (0,0,0)
	float alignPos = (METER * 0.5f);
	//Resize the vector to fit *length* vectors
	m_floor.resize(length);
	for (int x = 0; x < length; x++)
	{
		//Resize the vector to fit *width* objects
		//every ptr to a 3D model is initialized to be null
		m_floor[x].resize(width, nullptr);
		for (int y = 0; y < width; y++)
		{
			//Set the settings for every element in the 2d vector
			float offsetX = float(length / 2) * METER;
			float offsety = float(width / 2) * METER;
			float posX = float(x) * METER;
			float posY = float(y) * METER;

			//Create new 3D objects to fill the 2D vector
			m_floor[x][y] = std::make_shared<Lime::Model::Model3D>(id);
			m_floor[x][y]->Scale(10.0f, 2.5f, 10.0f);
			m_floor[x][y]->SetPosition(alignPos + posX - offsetX, METER * -0.125f, alignPos + posY - offsety);
			m_floor[x][y]->SetTexture(texture);

			//Dont forget to add to render batch
			m_render->Add3DModel(m_floor[x][y]);
		}
	}
}

void GameStates::MapEditor::UpdateCursor(float time, std::shared_ptr<Lime::InputManager>& input)
{
	//Static means the variable will stay relevant even after the end of the function and will
	//-have it's prev value if called again
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

	//Adds to the new block vector described in the header file
	m_newBlocks.push_back(newBlock);

	//Needs to be added to the render batch
	m_render->Add3DModel(newBlock);
}
