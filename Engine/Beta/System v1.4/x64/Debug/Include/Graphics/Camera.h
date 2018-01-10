#pragma once
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "DLLSettings.h"
#include "Model.h"

namespace Graphics
{
	struct CameraSettings
	{
		GRAPHICS_DLL_API CameraSettings();
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
		std::shared_ptr<Model> m_model;
	};

	class Camera
	{
	public:
		GRAPHICS_DLL_API Camera(CameraSettings defaultSettings = CameraSettings());
		GRAPHICS_DLL_API void Reset();
		GRAPHICS_DLL_API void SetDefaultSettings(CameraSettings info);
		GRAPHICS_DLL_API void AttachToModel(std::shared_ptr<Model> model);
		GRAPHICS_DLL_API void SetViewDistance(float nearPlane);
		GRAPHICS_DLL_API void SetFOV(float fov);
		GRAPHICS_DLL_API void Move(float x, float y, float z);
		GRAPHICS_DLL_API void SetPosition(glm::vec3 position);
		GRAPHICS_DLL_API void Rotate(float x, float y, float z);
		GRAPHICS_DLL_API void SetRotation(glm::vec3 rotation);
		GRAPHICS_DLL_API void SetResolution(uint32_t width, uint32_t height);
		GRAPHICS_DLL_API void Zoom(float x);
		GRAPHICS_DLL_API glm::vec3 GetPosition();
		GRAPHICS_DLL_API glm::mat4 GetViewMatrix();
		GRAPHICS_DLL_API glm::mat4 GetIdentityMatrix();
		GRAPHICS_DLL_API glm::mat4 Get3DProjectionMatrix();
		GRAPHICS_DLL_API glm::mat4 Get2DProjectionMatrix();
		GRAPHICS_DLL_API float GetFarPlane();
		GRAPHICS_DLL_API unsigned int GetWindowWidth();
		GRAPHICS_DLL_API unsigned int GetWindowHeight();
		GRAPHICS_DLL_API void EnforceBounds(bool val);
	private:
		void SetAngeInBounds(float& angle);
		void CalculateAspectRatio();
		void CreateViewMatrix();
		void Create3DProjectionMatrix();
		void Create2DProjectionMatrix();
		float CalculateVertDistance();
		float CalculateHorisDistance();
		void CalculatePosition(float horizontalDistance, float verticalDistance);

		bool m_bUsingBounds;
		CameraSettings m_defaultSettings;
		CameraSettings m_currentSettings;
	};


}