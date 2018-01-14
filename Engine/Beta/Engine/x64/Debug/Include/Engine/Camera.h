#pragma once
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "DLLSettings.h"
#include "Model.h"

namespace Engine
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
		__engine_decl CameraSettings();

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
		__engine_decl Camera(CameraSettings defaultSettings = CameraSettings());
		__engine_decl void Reset();
		__engine_decl void SetDefaultSettings(CameraSettings info);
		__engine_decl void AttachToModel(const Model& model);
		__engine_decl void SetViewDistance(float nearPlane);
		__engine_decl void SetFOV(float fov);
		__engine_decl void Move(float x, float y, float z);
		__engine_decl void SetPosition(glm::vec3 position);
		__engine_decl void Rotate(float x, float y, float z);
		__engine_decl void SetRotation(glm::vec3 rotation);
		__engine_decl void SetResolution(float width, float height);
		__engine_decl void Zoom(float x);
		__engine_decl glm::vec3 GetPosition();
		__engine_decl glm::mat4 GetViewMatrix();
		__engine_decl glm::mat4 GetIdentityMatrix();
		__engine_decl glm::mat4 Get3DProjectionMatrix();
		__engine_decl float GetFarPlane();
		__engine_decl float GetWindowWidth();
		__engine_decl float GetWindowHeight();
		__engine_decl void EnforceBounds(bool val);
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