#include "Polygon.h"

const unsigned int Graphics::Polygon::GetVertexCount()
{
	return (const unsigned int)m_vertices.size();
}

const unsigned int Graphics::Polygon::GetIndexCount()
{
	return (const unsigned int)m_indices.size();
}