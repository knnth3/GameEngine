#pragma once
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <Lime_Engine\EngineApp.h>
#include <Lime_Engine\Cube.h>
#include <Lime_Engine\Lime_String.h>
#include <chrono>
#include "Populace\Populace.h"


using namespace LIME_ENGINE;
class Engine_App:
	public EngineApp
{
public:
	Engine_App(uint16_t windowWidth, uint16_t windowHeight, std::string programPath);

	//Overloaded functions
	virtual void OnStartUp() override;
	virtual void OnShutdown() override;
	virtual void OnTick() override;

private:
	void Update(float elapsed);
	void Render();

	//used to calculate time elapsed
	int m_fps = 0;
	float movespeed;
	bool stress;
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

	std::vector<Graphics::Model> m_models;
	std::shared_ptr<Shapes::Cube> m_cube;
	Lime_String m_str;
	Lime_String m_fpsStr;
};

