#include "Engine_App.h"
#include <Graphics\MeshLoader.h>
using namespace Graphics;
using namespace std;

Engine_App::Engine_App(uint16_t windowWidth, uint16_t windowHeight, std::string programPath):
	EngineApp(windowWidth, windowHeight, programPath)
{
	//Local independent variables initialize here
	movespeed = 1.0f;
}

void Engine_App::OnStartUp()
{
	//Not nessesary(Customizes the console). Use ` button to open
	auto console = this->GetConsole();
	console->SetOpacity(0.5f);
	console->SetColor(0.1f, 0.1f, 0.1f);

	//Make objects here
	m_cube = std::make_shared<Shapes::Cube>(100.0f, 100.0f, 20.0f);
	//Scale not working correctly here (scale manually)^
	//m_cube->Scale(1.0f, 1.0f, 5.0f);
	m_cube->SetColor(0.0f, 0.0f, 1.0f, 1.0f);

	//Sphere
	MeshID sphereID = MeshLoader::LoadModel("Assets/Models/Sphere.obj");
	m_sphere = Graphics::Model(sphereID);
	m_sphere.Scale(100.0f, 100.0f, 100.0f);


	//This allows camera to revolve around the model
	this->GetCamera()->AttachToModel(m_cube);

	//Use this instead of cout
	console->Log("Controls:", LOG_GREEN);
	console->Log("Left = D", LOG_GREEN);
	console->Log("Right = A", LOG_GREEN);

	//Do not mess with (presets timer)
	m_fpsStr.SetPosition(0.0f, 0.0f);
	start = std::chrono::system_clock::now();
	end = start;
}

void Engine_App::OnShutdown()
{
}

void Engine_App::OnTick()
{
	//Do not mess with
	//Calculates the tick time
	static float count = 0;
	end = std::chrono::system_clock::now();
	std::chrono::duration<float> time = end - start;
	start = end;
	count += time.count();
	if (count > 1.0f)
	{
		m_fps = (int)(1.0f / time.count());
		count = 0.0f;
		m_fpsStr = "FPS:" + std::to_string(m_fps);
	}
	Update(time.count());

	Render();
}

void Engine_App::Update(float elapsed)
{
	//Everything that needs an update is here
	//Set variables
	static float rotation = 0.0f;
	float value = movespeed * elapsed;
	rotation += value;
	auto& input = GetInputManager();
	auto& camera = GetCamera();

	//Update 
	if (input->KeyStatus(System::KEY_a))
		camera->Rotate(0.0f, value, 0.0f);
	if (input->KeyStatus(System::KEY_d))
		camera->Rotate(0.0f, -value, 0.0f);
	if (input->KeyStatus(System::KEY_w))
		camera->Rotate(value, 0.0f, 0.0f);
	if (input->KeyStatus(System::KEY_s))
		camera->Rotate(-value, 0.0f, 0.0f);

	//m_cube->Rotate(rotation, 0.0f, 0.0f);
	//auto mouse = input->GetMouse3DPosition();
	//cout << mouse.x << "    :   " << mouse.z << endl;
}

void Engine_App::Render()
{
	//Draw (order matters!)
	auto graphics = this->GetGraphicsDevice();
	graphics->BeginScene(0.2f, 0.2f, 0.2f);

	//Objects
	graphics->Draw(m_sphere);
	graphics->Draw(*m_cube.get());
	m_fpsStr.Draw(graphics);

	graphics->EndScene();
}