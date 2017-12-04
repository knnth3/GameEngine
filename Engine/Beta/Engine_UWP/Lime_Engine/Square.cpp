#include "Square.h"

using namespace Graphics;

MeshID LIME_ENGINE::Shapes::Square::m_meshID = -1;
LIME_ENGINE::Shapes::Square::Square()
{
	LoadMesh();
	this->SetDrawStyle(DrawStyle::TRIANGLE_2D);
	SetDimensions(1.0f, 1.0f);
}

LIME_ENGINE::Shapes::Square::Square(float length, float width)
{
	LoadMesh();
	this->SetDrawStyle(DrawStyle::TRIANGLE_2D);
	SetDimensions(length, width);
}

void LIME_ENGINE::Shapes::Square::SetDimensions(float length, float width)
{
	m_length = length;
	m_width = width;
	ScaleToDimensions();

}

void LIME_ENGINE::Shapes::Square::SetLength(float length)
{
	m_length = length;
	ScaleToDimensions();
}

void LIME_ENGINE::Shapes::Square::SetWidth(float width)
{
	m_width = width;
	ScaleToDimensions();
}

void LIME_ENGINE::Shapes::Square::Wireframe(bool value)
{
	if (value)
		this->SetRSS(RSS_WIREFRAME);
	else
		this->SetRSS(RSS_DEFAULT);
}

void LIME_ENGINE::Shapes::Square::Scale(float length, float width)
{
	this->Model::Scale(m_length * length, m_width * width, 0.0f);
}

void LIME_ENGINE::Shapes::Square::LoadMesh()
{
	if (m_meshID == -1)
	{
		std::vector<Vertex> square_vertices
		{
			{ { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } },
			{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 0.0f} },
			{ { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } },

			{ { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } },
			{ { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } },
			{ { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } },
		};
		std::vector<Index> square_indices = { 0, 1, 2, 3, 4, 5 };

		m_meshID = MeshLoader::LoadModel(square_vertices, square_indices, "Square");
	}
	this->SetMesh(m_meshID);
}

void LIME_ENGINE::Shapes::Square::ScaleToDimensions()
{
	this->Model::Scale(m_length, m_width, 0.0f);
}
