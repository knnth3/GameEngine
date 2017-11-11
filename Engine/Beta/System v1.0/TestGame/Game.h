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
	std::shared_ptr<Model> m_character;
	std::shared_ptr<Model> m_character2;
	std::shared_ptr<Model> m_character3;
	std::vector<Graphics::Vertex> m_2dModel;
	std::vector<Graphics::Index> m_2dModelIndex;
};

