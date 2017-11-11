#pragma once
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "DLLSettings.h"
#include "Model3D.h"

namespace Graphics
{
	struct CameraSettings
	{
		DLL_API CameraSettings();
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
		std::shared_ptr<Model::Model3D> m_model;
	};

	class Camera
	{
	public:
		DLL_API Camera(CameraSettings defaultSettings = CameraSettings());
		DLL_API void Reset();
		DLL_API void SetDefaultSettings(CameraSettings info);
		DLL_API void AttachToModel(std::shared_ptr<Model::Model3D>& model);
		DLL_API void SetViewDistance(float nearPlane);
		DLL_API void SetFOV(float fov);
		DLL_API void Move(float x, float y, float z);
		DLL_API void SetPosition(glm::vec3 position);
		DLL_API void Rotate(float x, float y, float z);
		DLL_API void SetRotation(glm::vec3 rotation);
		DLL_API void SetResolution(uint32_t width, uint32_t height);
		DLL_API void Zoom(float x);
		DLL_API glm::vec3 GetPosition();
		DLL_API glm::mat4 GetViewMatrix();
		DLL_API glm::mat4 GetIdentityMatrix();
		DLL_API glm::mat4 Get3DProjectionMatrix();
		DLL_API glm::mat4 Get2DProjectionMatrix();
		DLL_API float GetFarPlane();
		DLL_API unsigned int GetWindowWidth();
		DLL_API unsigned int GetWindowHeight();
	private:
		void CalculateAspectRatio();
		void CreateViewMatrix();
		void Create3DProjectionMatrix();
		void Create2DProjectionMatrix();
		float CalculateVertDistance();
		float CalculateHorisDistance();
		void CalculatePosition(float horizontalDistance, float verticalDistance);

		CameraSettings m_defaultSettings;
		CameraSettings m_currentSettings;
	};


}