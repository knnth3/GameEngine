#pragma once
#include <memory>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "Primitives.h"



namespace Lime
{
	struct CAMERA_INFO
	{
		float m_fov = 0.25f* 3.14f;
		float m_aspectRatio = 0.0f;
		float m_nearPlane = 1.0f;
		float m_farPlane = 1000.0f;
		float m_distanceFromObject = 10.0f;
		float m_angleAroundPlayer = 0.0f;
		glm::mat4 m_view = glm::mat4(0.0f);
		glm::mat4 m_projection = glm::mat4(0.0f);
		glm::vec3 m_position = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_targetPos = glm::vec3(0.0f);
		glm::vec3 m_upDirection = glm::vec3(0.0f);
		glm::vec3 m_rotation = glm::vec3(0.0f);
		UINT m_xResolution = 0;
		UINT m_yResolution = 0;
		std::shared_ptr<Model3D> m_model = nullptr;
		bool m_bPlayerAttached = false;
	};

    class Camera
    {
    public:
        DLL_EXPORT Camera() = default;
        DLL_EXPORT void Initialize(UINT windowWidth, UINT windowHeight);
        DLL_EXPORT void AttachToModel(std::shared_ptr<Model3D>& model);
        DLL_EXPORT void SetViewDistance(float near, float far);
        DLL_EXPORT void SetFOV(float fov);
        DLL_EXPORT void Move(float x, float y, float z);
        DLL_EXPORT void SetPosition(glm::vec3 position);
		DLL_EXPORT void Rotate(float x, float y, float z);
        DLL_EXPORT void SetRotation(glm::vec3 rotation);
        DLL_EXPORT void SetResolution(UINT width, UINT height);
        DLL_EXPORT void ZoomIn(float x);
        DLL_EXPORT void AddPitch(float pitch);
        DLL_EXPORT void AddYaw(float yaw);
        DLL_EXPORT glm::vec3 GetPosition();
        DLL_EXPORT glm::mat4 GetViewMatrix();
        DLL_EXPORT glm::mat4 GetProjectionMatrix();
        DLL_EXPORT float GetFarPlane();
        DLL_EXPORT unsigned int GetWindowWidth();
        DLL_EXPORT unsigned int GetWindowHeight();
    private:
        void CreateViewMatrix();
        void CreateProjectionMatrix();
		float CalculateVertDistance();
		float CalculateHorisDistance();
        void CalculatePosition(float horizontalDistance, float verticalDistance);

		CAMERA_INFO m_info;
    };


}