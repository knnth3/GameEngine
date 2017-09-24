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
		DLL_EXPORT XMMATRIX GetWorldMaxrix();
		DLL_EXPORT XMMATRIX GetViewMatrix();
		DLL_EXPORT XMMATRIX GetProjectionMatrix();
		DLL_EXPORT XMMATRIX GetWVPnMatrix();
	private:
		int m_width;
		int m_height;
		//radians along the y-axis
		float m_fov;
		float m_aspectRatio;
		float m_nearPlane;
		float m_farPlane;
		XMMATRIX m_projection;
		XMMATRIX m_view;
		XMVECTOR m_position;
		XMVECTOR m_targetPos;
		XMVECTOR m_UpDirection;
	};
}

