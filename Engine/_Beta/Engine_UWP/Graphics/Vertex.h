#pragma once
#include "DllSettings.h"

namespace Graphics
{
	struct Vertex
	{
		Vertex();
		glm::vec3 m_position;
		glm::vec2 m_uv;
		glm::vec4 m_normal;
		glm::vec3 m_tangent;
		glm::vec3 m_binormal;
		glm::vec3 m_color;
	};
}