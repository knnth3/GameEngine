#pragma once
#include <DirectXMath.h>
#include "Primitives.h"

using namespace DirectX;
namespace Lime
{
	class DX11Camera
	{
	public:
		DLL_EXPORT DX11Camera(int width, int height);
		DLL_EXPORT ~DX11Camera();
		DLL_EXPORT glm::mat4 GetWorldMaxrix();
		DLL_EXPORT glm::mat4 GetViewMatrix();
		DLL_EXPORT glm::mat4 GetProjectionMatrix();
		DLL_EXPORT glm::mat4 GetWVPnMatrix();
	private:
		int m_width;
		int m_height;
		//radians along the y-axis
		float m_fov;
		float m_aspectRatio;
		float m_nearPlane;
		float m_farPlane;
		glm::mat4 m_projection;
		glm::mat4 m_view;
		glm::vec3 m_position;
		glm::vec3 m_targetPos;
		glm::vec3 m_UpDirection;
	};
}

