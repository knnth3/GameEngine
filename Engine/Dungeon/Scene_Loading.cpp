#include "Scene_Loading.h"



Scenes::Scene_Loading::Scene_Loading(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera):
	Scene(rend, camera)
{

}

Scenes::Scene_Loading::~Scene_Loading()
{
}

void Scenes::Scene_Loading::Initialize()
{
}

void Scenes::Scene_Loading::Update(float time, std::shared_ptr<Lime::InputManager>& input)
{
}
