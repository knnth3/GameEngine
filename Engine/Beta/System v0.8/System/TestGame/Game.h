#pragma once
#include <System\System.h>
using namespace Graphics;

class Game:
	public System::App
{
public:
	Game(uint16_t windowWidth, uint16_t windowHeight);

	//state changes
	virtual void Tick() override;
	virtual void OnInitialize() override;

private:
	std::shared_ptr<Model::Model3D> m_character;
};

