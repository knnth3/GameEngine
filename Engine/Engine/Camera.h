#pragma once
#include <memory>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "Primitives.h"

namespace Lime
{

    class Camera
    {
    public:
        DLL_EXPORT Camera();
        DLL_EXPORT void Initialize(std::shared_ptr<unsigned int>& windowWidth, std::shared_ptr<unsigned int>& windowHeight);
        DLL_EXPORT void AttachToModel(std::shared_ptr<Model>& model);
        DLL_EXPORT void SetViewDistance(float near, float far);
        DLL_EXPORT void SetFOV(float fov);
        DLL_EXPORT void Update();
        DLL_EXPORT void Move(float x, float y, float z);
        DLL_EXPORT void SetPosition(glm::vec3 position);
        DLL_EXPORT void Rotate(float x, float y, float z);
        DLL_EXPORT void SetRotation(glm::vec3 rotation);
        DLL_EXPORT void RefreshAspectRatio();
        DLL_EXPORT void ZoomIn(float x);
        DLL_EXPORT void AddPitch(float pitch);
        DLL_EXPORT void AddYaw(float yaw);
        DLL_EXPORT glm::vec3 GetPosition();
        DLL_EXPORT glm::mat4 GetViewMatrix();
        DLL_EXPORT glm::mat4 GetProjectiomMatrix();
        DLL_EXPORT float GetFarPlane();
        DLL_EXPORT unsigned int GetWindowWidth();
        DLL_EXPORT unsigned int GetWindowHeight();
    private:
        void CreateViewMatrix(bool overrideUpdateCall = false);
        void CreateProjectionMatrix(bool overrideUpdateCall = false);
        void CalculatePosition(float horizontalDistance, float verticalDistance);
        float CalculateVertDistance();
        float CalculateHorisDistance();

        bool m_bPlayerAttached = false;
        bool m_bInvertXRot = true;
        bool m_bViewUpdate = false;
        bool m_bProjectionUpdate = false;
        int m_PrevWindowWidth = 0;
        int m_PrevWindowHeight = 0;
        float m_FOV = 60;
        float m_AspectRatio = 0.0f;
        float m_NearPlane = 0.1f;
        float m_FarPlane = 300.0f;
        float m_DistanceFromObject = 6.0f;
        float m_AngleAroundPlayer = 0.f;
        glm::vec3 m_Position;
        //Pitch->Yaw->Roll
        glm::vec3 m_Rotation;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        std::shared_ptr<unsigned int> m_WindowWidth;
        std::shared_ptr<unsigned int> m_WindowHeight;
        std::shared_ptr<Model> m_Model;
    };


}