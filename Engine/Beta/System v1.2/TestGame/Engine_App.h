#pragma once
#include <Lime_Engine\EngineApp.h>
#include <Lime_Engine\Cube.h>
#include <Lime_Engine\Lime_String.h>
#include <chrono>


using namespace LIME_ENGINE;
class Engine_App:
	public EngineApp
{
public:
	Engine_App(uint16_t windowWidth, uint16_t windowHeight);
	~Engine_App();

	//Overloaded functions
	virtual void OnStartUp() override;
	virtual void OnTick() override;

private:
	void Update(float elapsed);
	void Render();
	void StressTest(bool val, int num = 0);

	//used to calculate time elapsed
	int m_fps = 0;
	float movespeed;
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

	std::vector<std::shared_ptr<Graphics::Model>> m_models;
	std::shared_ptr<Shapes::Cube> m_cube;
	Lime_String m_str;
	Lime_String m_fpsStr;
};

