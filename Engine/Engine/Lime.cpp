#include <glm\gtx\transform.hpp>
#include "Lime.h"

Lime::WorldLight::WorldLight()
{
	m_direction =  glm::vec3(0.0f, 1.0f, 0.0f);
	m_specularColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_ambientColor = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
	m_diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_specularPower = 60.0f;
}

void Lime::GetFileExt(const std::string& path, std::string & ext)
{
	//removes the '.'
	size_t extPoint = path.find_last_of('.') + 1;

	try
	{
		if (extPoint)
			ext = path.substr(extPoint, path.size() - 1);
	}
	catch (...) //General (catch all)
	{
		//Log error
		ext = "";
	}
}
