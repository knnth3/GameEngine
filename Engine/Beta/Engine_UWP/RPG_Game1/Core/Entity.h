#pragma once
#include <Lime\Model3D.h>

//class Entity
//{
//public:
//	Entity(std::shared_ptr<Lime::Model::Model3D> model);
//	std::shared_ptr<Lime::Model::Model3D> GetModel();
//private:
//	std::shared_ptr<Lime::Model::Model3D> m_model;
//};

using namespace Lime::Model;
using namespace std;

struct Entity
{
	Entity();
	void DrawModel();
	shared_ptr<Model3D> m_model;
};

