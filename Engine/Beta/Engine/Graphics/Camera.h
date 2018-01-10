#pragma once
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "DLLSettings.h"
#include "Model.h"

namespace Graphics
{
	struct Position
	{
		Position();
		bool Empty();
		glm::vec3 GetPosition();
		vec3_ptr m_position;
	};

	struct CameraSettings
	{
		GRAPHICS_DLL_API CameraSettings();

		float m_fov;
		float m_aspectRatio;
		float m_nearDistance;
		float m_farDistance;
		float m_distanceFromObject;
		float m_angleAroundPlayer;
		float m_cameraHeight;
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::vec3 m_position;
		glm::vec3 m_upDirection;
		glm::vec3 m_rotations;
		float m_screenWidth;
		float m_screenHeight;
		Position m_focusPoint;
	};

	class Camera
	{
	public:
		GRAPHICS_DLL_API Camera(CameraSettings defaultSettings = CameraSettings());
		GRAPHICS_DLL_API void Reset();
		GRAPHICS_DLL_API void SetDefaultSettings(CameraSettings info);
		GRAPHICS_DLL_API void AttachToModel(const Model& model);
		GRAPHICS_DLL_API void SetViewDistance(float nearPlane);
		GRAPHICS_DLL_API void SetFOV(float fov);
		GRAPHICS_DLL_API void Move(float x, float y, float z);
		GRAPHICS_DLL_API void SetPosition(glm::vec3 position);
		GRAPHICS_DLL_API void Rotate(float x, float y, float z);
		GRAPHICS_DLL_API void SetRotation(glm::vec3 rotation);
		GRAPHICS_DLL_API void SetResolution(float width, float height);
		GRAPHICS_DLL_API void Zoom(float x);
		GRAPHICS_DLL_API glm::vec3 GetPosition();
		GRAPHICS_DLL_API glm::mat4 GetViewMatrix();
		GRAPHICS_DLL_API glm::mat4 GetIdentityMatrix();
		GRAPHICS_DLL_API glm::mat4 Get3DProjectionMatrix();
		GRAPHICS_DLL_API float GetFarPlane();
		GRAPHICS_DLL_API float GetWindowWidth();
		GRAPHICS_DLL_API float GetWindowHeight();
		GRAPHICS_DLL_API void EnforceBounds(bool val);
	private:
		void SetAngeInBounds(float& angle);
		void CalculateAspectRatio();
		void CreateViewMatrix();
		void Create3DProjectionMatrix();
		float CalculateVertDistance();
		float CalculateHorisDistance();
		void CalculatePosition(float horizontalDistance, float verticalDistance);
		void CalculateClippingPlanes();

		bool m_bUsingBounds;
		CameraSettings m_defaults;
		CameraSettings m_active;
	};


}