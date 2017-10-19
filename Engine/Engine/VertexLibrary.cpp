#include "VertexLibrary.h"



bool Lime::Model::VertexLibrary::empty()
{
	size_t vert = m_vertices.size();
	size_t ind = m_indices.size();

	size_t result = vert*ind;
	if (result == 0)
		return true;

	return false;
}

void Lime::Model::VertexLibrary::clear()
{
	m_cachedIDs.clear();
	m_vertices.clear();
	m_indices.clear();
	m_3Dmodels.clear();
	m_2Dmodels.clear();
	VertCountOffset = 0;
	IndCountOffset = 0;
}

size_t Lime::Model::VertexLibrary::size()
{
	return m_3Dmodels.size();
}

size_t Lime::Model::VertexLibrary::size2D()
{
	return m_2Dmodels.size();
}

std::shared_ptr<Lime::Model::Model2D>& Lime::Model::VertexLibrary::at2D(const size_t index)
{
	try
	{
		return m_2Dmodels.at(index);
	}
	catch (...)
	{
		//Catch plzzzzzz
		return m_2Dmodels.at(0);
	}
}

void Lime::Model::VertexLibrary::Add3DModel(std::shared_ptr<Model::Model3D>& model)
{
	bool similar = false;
	for (auto const& id : m_cachedIDs)
	{
		if (id == model->m_mesh->objectID)
		{
			similar = true;
		}
	}
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	model->m_mesh->GetBuffers(vertices, indices);
	if (!similar)
	{
		m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
		m_indices.insert(m_indices.end(), indices.begin(), indices.end());

		model->m_mesh->vertOffset = VertCountOffset;
		model->m_mesh->indiciOffset = IndCountOffset;
		VertCountOffset += (int)vertices.size();
		IndCountOffset += (int)indices.size();
		m_cachedIDs.push_back(model->m_mesh->objectID);
	}


	m_3Dmodels.push_back(model);
}

void Lime::Model::VertexLibrary::Add2DModel(std::shared_ptr<Model::Model2D>& model)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	model->m_mesh->GetBuffers(vertices, indices);
	m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
	m_indices.insert(m_indices.end(), indices.begin(), indices.end());

	model->m_mesh->vertOffset = VertCountOffset;
	model->m_mesh->indiciOffset = IndCountOffset;
	VertCountOffset += (int)vertices.size();
	IndCountOffset += (int)indices.size();

	m_2Dmodels.push_back(model);
}

const void * Lime::Model::VertexLibrary::VertexData()
{
	return m_vertices.data();
}

const void * Lime::Model::VertexLibrary::IndexData()
{
	return m_indices.data();
}

uint32_t Lime::Model::VertexLibrary::VertexDataSize()
{
	return (uint32_t)m_vertices.size();
}

uint32_t Lime::Model::VertexLibrary::IndexDataSize()
{
	return (uint32_t)m_indices.size();
}

std::shared_ptr<Lime::Model::Model3D>& Lime::Model::VertexLibrary::operator[](const size_t index)
{
	try
	{
		return m_3Dmodels[index];
	}
	catch (...)
	{
		//Catch plzzzzzz
		return m_3Dmodels[0];
	}
}
