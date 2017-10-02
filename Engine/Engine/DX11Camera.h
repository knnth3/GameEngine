#pragma once
#include <DirectXMath.h>
#include "Primitives.h"

using namespace DirectX;
namespace Lime
{
	class DX11Camera
	{
	public:
		AppDLL_API DX11Camera(int width, int height);
		AppDLL_API ~DX11Camera();
		AppDLL_API glm::mat4 GetWorldMaxrix();
		AppDLL_API glm::mat4 GetViewMatrix();
		AppDLL_API glm::mat4 GetProjectionMatrix();
		AppDLL_API glm::mat4 GetWVPnMatrix();
		AppDLL_API glm::vec3 GetPosition();
		AppDLL_API void SetAspectRatio(const UINT width, const UINT height);
		AppDLL_API void SetPosition(float x, float y, float z);
		AppDLL_API glm::vec3 GetUpDirection();
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

