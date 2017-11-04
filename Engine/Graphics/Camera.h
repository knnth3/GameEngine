#pragma once
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "DLLSettings.h"

namespace Graphics
{
	struct CameraInfo
	{
		AppDLL_API CameraInfo();
		float m_fov;
		float m_aspectRatio;
		float m_nearPlane;
		float m_farPlane;
		float m_distanceFromObject;
		float m_angleAroundPlayer;
		float m_cameraHeight;
		glm::mat4 m_view;
		glm::mat4 m_3Dprojection;
		glm::mat4 m_2Dprojection;
		glm::vec3 m_position;
		glm::vec3 m_upDirection;
		glm::vec3 m_rotation;
		uint32_t m_xResolution;
		uint32_t m_yResolution;
		std::shared_ptr<glm::vec3> m_modelPosition;
	};

	class Camera
	{
	public:
		AppDLL_API Camera(CameraInfo info = CameraInfo());
		AppDLL_API void Initialize(uint32_t windowWidth, uint32_t windowHeight);
		AppDLL_API void AttachToModel(std::shared_ptr<glm::vec3>& modelPos);
		AppDLL_API void SetViewDistance(float nearPlane);
		AppDLL_API void SetFOV(float fov);
		AppDLL_API void Move(float x, float y, float z);
		AppDLL_API void SetPosition(glm::vec3 position);
		AppDLL_API void Rotate(float x, float y, float z);
		AppDLL_API void SetRotation(glm::vec3 rotation);
		AppDLL_API void SetResolution(uint32_t width, uint32_t height);
		AppDLL_API void Zoom(float x);
		AppDLL_API glm::vec3 GetPosition();
		AppDLL_API glm::mat4 GetViewMatrix();
		AppDLL_API glm::mat4 GetIdentityMatrix();
		AppDLL_API glm::mat4 Get3DProjectionMatrix();
		AppDLL_API glm::mat4 Get2DProjectionMatrix();
		AppDLL_API float GetFarPlane();
		AppDLL_API unsigned int GetWindowWidth();
		AppDLL_API unsigned int GetWindowHeight();
	private:
		void CreateViewMatrix();
		void Create3DProjectionMatrix();
		void Create2DProjectionMatrix();
		float CalculateVertDistance();
		float CalculateHorisDistance();
		void CalculatePosition(float horizontalDistance, float verticalDistance);

		const CameraInfo m_defaultInfo;
		CameraInfo m_info;
	};


}