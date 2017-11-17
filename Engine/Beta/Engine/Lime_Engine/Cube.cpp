#include "Cube.h"

using namespace Graphics;
LIME_ENGINE::Shapes::Cube::Cube()
{
	this->SetDrawStyle(DrawStyle::TRIANGLE_3D);
	this->SetRSS(RSS_NO_CULL);
	SetDimensions(1.0f, 1.0f, 1.0f);
}

LIME_ENGINE::Shapes::Cube::Cube(float length, float width, float height)
{
	this->SetDrawStyle(DrawStyle::TRIANGLE_3D);
	SetDimensions(length, width, height);
}

void LIME_ENGINE::Shapes::Cube::SetDimensions(float length, float width, float height)
{
	m_length = length;
	m_width = width;
	m_height = height;
	ScaleToDimensions();
}

void LIME_ENGINE::Shapes::Cube::SetLength(float length)
{
	m_length = length;
	ScaleToDimensions();
}

void LIME_ENGINE::Shapes::Cube::SetWidth(float width)
{
	m_width = width;
	ScaleToDimensions();
}

void LIME_ENGINE::Shapes::Cube::SetHeight(float height)
{
	m_height = height;
	ScaleToDimensions();
}

void LIME_ENGINE::Shapes::Cube::Wireframe(bool value)
{
	if(value)
		this->SetRSS(RSS_WIREFRAME);
	else
		this->SetRSS(RSS_NO_CULL);
}

void LIME_ENGINE::Shapes::Cube::Scale(float length, float width, float height)
{
	this->Model::Scale(m_length * length, m_height * height, m_width * width);
}

void LIME_ENGINE::Shapes::Cube::ScaleToDimensions()
{
	this->Model::Scale(m_length, m_height, m_width);
}
