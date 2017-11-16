#include "Engine_App.h"


using namespace Graphics;
Engine_App::Engine_App(uint16_t windowWidth, uint16_t windowHeight):
	EngineApp(windowWidth, windowHeight)
{
	movespeed = 1.0f;
}

Engine_App::~Engine_App()
{

}

void Engine_App::OnStartUp()
{
	auto console = this->GetConsole();
	console->SetOpacity(0.5f);
	console->SetColor(0.1f, 0.1f, 0.1f);
	m_cube = std::make_shared<Shapes::Cube>(20.0f, 20.0f, 1.0f);
	this->GetCamera()->AttachToModel(m_cube);
	m_cube->Scale(1.0f, 1.0f, 5.0f);
	m_str = "FPS:";
	m_str.SetColor(0.0f, 1.0f, 0.0f);
	m_fpsStr.SetPosition(60, 0.0f);
	console->Log("STRESS TEST: Press ENTER to switch on\\off", LOG_RED);
	start = std::chrono::system_clock::now();
	end = start;
}

void Engine_App::OnTick()
{
	static int count = 0;
	count++;
	end = std::chrono::system_clock::now();
	std::chrono::duration<float> time = end - start;
	start = end;
	if (count >= 100)
	{
		m_fps = (int)(1.0f / time.count());
		count = 0;
		m_fpsStr = std::to_string(m_fps);
		std::cout << m_fps << std::endl;
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
	static bool stress = false;

	//Update
	auto input = this->GetInputManager();
	if (input->KeyPressed(System::KEY_w))
		movespeed *= 10.0f;
	if (input->KeyPressed(System::KEY_s))
		movespeed = movespeed / 10.0f;
	if (input->KeyPressed(System::KEY_RETURN))
	{
		stress = !stress;
		StressTest(stress, 100);
	}

	m_cube->Rotate(rotation, 0.0f, 0.0f);
}

void Engine_App::Render()
{
	//Draw (order matters!)
	auto graphics = this->GetGraphicsDevice();
	graphics->BeginScene(0.3f, 0.3f, 0.3f);
	for (auto x : m_models)
		graphics->Draw(x);
	m_str.Draw(graphics);
	m_fpsStr.Draw(graphics);
	graphics->Draw(m_cube);
	graphics->EndScene();
}

void Engine_App::StressTest(bool val, int num)
{
	if (val)
	{
		for (int x = 0; x < num; x++)
		{
			auto m_character = std::make_shared<Model>();
			m_character->SetDrawStyle(DrawStyle::TRIANGLE_3D);
			m_character->SetPosition(0.0f, 0.0f, 10.0f);
			m_models.push_back(m_character);
		}
	}
	else
	{
		m_models.clear();
	}
}
