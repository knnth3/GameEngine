#include "pch.h"
#include "Entity.h"
#include <Lime\ModelLoader.h>

Entity::Entity()
{
	MeshID id = MeshLoader::LoadModel("");
	m_model = std::make_shared<Model3D>(id);
}

void Entity::DrawModel()
{
	if (m_model)
		m_model->Draw();
}
