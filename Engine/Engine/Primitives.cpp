#include <glm\gtx\transform.hpp>
#include "Primitives.h"


void Lime::DiffuseLight::SetDirection(const glm::vec3 dir)
{
	m_direction = dir;
}

void Lime::DiffuseLight::SetColor(const glm::vec4 color)
{
	m_color = color;
}

const glm::vec3 Lime::DiffuseLight::GetDirection() const
{
	return m_direction;
}

const glm::vec4 Lime::DiffuseLight::GetColor() const
{
	return m_color;
}