#include "Vertex.h"

Graphics::Vertex::Vertex()
{
	m_position = glm::vec3(0.0f);
	m_uv = glm::vec2(0.0f);
	m_color = glm::vec3(1.0f);
	m_normal = glm::vec4(0.0f);
	m_tangent = glm::vec3(0.0f);
	m_binormal = glm::vec3(0.0f);
}
