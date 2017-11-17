#include "Engine_App.h"

using namespace Graphics;
using namespace std;
Engine_App::Engine_App(uint16_t windowWidth, uint16_t windowHeight, std::string programPath):
	EngineApp(windowWidth, windowHeight, programPath)
{
	movespeed = 1.0f;
	stress = false;
	bool result = PL_Initialize(programPath);
	if (!result)
		CloseApp();
	m_id = PL_CreateActor("Eric");
}

void Engine_App::OnStartUp()
{
	auto console = this->GetConsole();
	console->SetOpacity(0.5f);
	console->SetColor(0.1f, 0.1f, 0.1f);
	m_cube = std::make_shared<Shapes::Cube>(20.0f, 20.0f, 1.0f);
	this->GetCamera()->AttachToModel(m_cube);
	m_cube->Scale(1.0f, 1.0f, 5.0f);
	m_cube->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_str = "FPS:";
	m_str.SetColor(0.0f, 1.0f, 0.0f);
	m_fpsStr.SetPosition(60, 0.0f);
	console->Log("STRESS TEST: Press ENTER to switch on\\off", LOG_RED);
	start = std::chrono::system_clock::now();
	end = start;

	float offset = 0.0f;
	Shapes::Cube m_character(2.0f, 2.0f, 2.0f);
	m_character.SetDrawStyle(DrawStyle::TRIANGLE_3D);
	for (int x = 0; x < 506; x++)
	{
		m_character.SetPosition(300.0f, 0.0f, offset);
		m_models.push_back(m_character);
		offset += 100.0f;
	}
}

void Engine_App::OnShutdown()
{
	PL::PL_ActorData data;
	bool result = PL_GetActorData(m_id, data);
	if (result)
	{
		cout << data.Name << endl;
		cout << "Current Health: " << data.CurrentHealth << endl;
		cout << "Total Health: " << data.MaxHealth << endl;
	}
	PL_WriteToDisk();
	PL_Close();
}

void Engine_App::OnTick()
{
	static float count = 0;
	end = std::chrono::system_clock::now();
	std::chrono::duration<float> time = end - start;
	start = end;
	count += time.count();
	if (count > 1.0f)
	{
		m_fps = (int)(1.0f / time.count());
		count = 0.0f;
		m_fpsStr = std::to_string(m_fps);
	}
	Update(time.count());

	Render();
}

void Engine_App::Update(float elapsed)
{
	//Set variables
	static float rotation = 0.0f;
	float value = movespeed * elapsed;
	rotation += value;

	//Update
	auto input = this->GetInputManager();
	if (input->KeyPressed(System::KEY_w))
		movespeed *= 10.0f;
	if (input->KeyPressed(System::KEY_s))
		movespeed = movespeed / 10.0f;
	if (input->KeyPressed(System::KEY_RETURN))
	{
		stress = !stress;
	}

	m_cube->Rotate(rotation, 0.0f, 0.0f);
}

void Engine_App::Render()
{
	//Draw (order matters!)
	auto graphics = this->GetGraphicsDevice();
	graphics->BeginScene(0.3f, 0.3f, 0.3f);
	graphics->Draw(*m_cube.get());
	m_str.Draw(graphics);
	if (stress)
	{
		for (auto x : m_models)
			graphics->Draw(x);
	}
	m_fpsStr.Draw(graphics);
	graphics->EndScene();
}