#pragma once
#include <memory>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "Shared.h"
#include "Model3D.h"

typedef uint32_t uint32_t;

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
		glm::mat4 m_3Dprojection = glm::mat4(0.0f);
		glm::mat4 m_2Dprojection = glm::mat4(0.0f);
		glm::vec3 m_position = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_targetPos = glm::vec3(0.0f);
		glm::vec3 m_upDirection = glm::vec3(0.0f);
		glm::vec3 m_rotation = glm::vec3(0.0f);
		uint32_t m_xResolution = 0;
		uint32_t m_yResolution = 0;
		std::shared_ptr<Model::Model3D> m_model = nullptr;
	};

    class Camera
    {
    public:
        AppDLL_API Camera() = default;
        AppDLL_API void Initialize(uint32_t windowWidth, uint32_t windowHeight);
        AppDLL_API void AttachToModel(std::shared_ptr<Model::Model3D>& model);
        AppDLL_API void SetViewDistance(float near, float far);
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
		CAMERA_INFO m_info;
    };


}