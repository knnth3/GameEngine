#include "Camera.h"

namespace Lime
{

    const float PI = 3.1415927f;

    Camera::Camera() :
        m_Position(0.0f, 1.0f, 0.0f),
        m_Rotation(0.0f, 0.0f, 0.0f),
        m_Model(nullptr)
    {
    }

    void Camera::Initialize(std::shared_ptr<unsigned int>& windowWidth, std::shared_ptr<unsigned int>& windowHeight)
    {
        if (windowWidth != nullptr && windowHeight != nullptr)
        {
            m_WindowWidth = windowWidth;
            m_WindowHeight = windowHeight;
        }
        else
        {
            m_WindowWidth = std::make_shared<unsigned int>(800);
            m_WindowHeight = std::make_shared<unsigned int>(600);
        }
        m_AspectRatio = ((float)*m_WindowWidth) / *m_WindowHeight;
        CreateProjectionMatrix(true);
        CreateViewMatrix(true);
    }

    void Camera::AttachToModel(std::shared_ptr<Model>& model)
    {
        m_Model = model;
    }

    void Camera::SetViewDistance(float near, float far)
    {
        m_NearPlane = near;
        m_FarPlane = far;
    }

    void Camera::SetFOV(float fov)
    {
        m_FOV = fov;
    }

    void Camera::Update()
    {
        float horis = CalculateHorisDistance();
        float vert = CalculateVertDistance();
        CalculatePosition(horis, vert);
        RefreshAspectRatio();
        CreateProjectionMatrix();
        CreateViewMatrix();
    }

    void Camera::Move(float x, float y, float z)
    {
        float speed = 5.0f;
        m_Position.x += x / speed;
        m_Position.y += y / speed;
        m_Position.z += z / speed;
    }

    void Camera::SetPosition(glm::vec3 position)
    {
        m_Position = position;
    }

    void Camera::Rotate(float x, float y, float z)
    {
        float circle = (2.0f*PI);
        float xLimitUp = (PI * 7.0f) / 16.0f;
        float xLimitDown = PI / 16.0f;
        m_Rotation.x += x;
        m_AngleAroundPlayer += y;

        if (m_Rotation.x > xLimitUp)
            m_Rotation.x = xLimitUp;

        if (m_Rotation.x < xLimitDown)
            m_Rotation.x = xLimitDown;

        if (m_AngleAroundPlayer >= circle)
            m_AngleAroundPlayer -= circle;

    }

    void Camera::SetRotation(glm::vec3 rotation)
    {
        float circle = (2.0f*PI);
        if (rotation.x >= circle)
            rotation.x -= circle;

        if (rotation.y >= circle)
            rotation.y -= circle;

        if (rotation.z >= circle)
            rotation.z -= circle;
        m_Rotation = rotation;
    }

    void Camera::RefreshAspectRatio()
    {
        if (*m_WindowWidth != m_PrevWindowWidth || *m_WindowHeight != m_PrevWindowHeight)
        {
            m_AspectRatio = ((float)*m_WindowWidth) / *m_WindowHeight;
        }
        m_PrevWindowWidth = *m_WindowWidth;
        m_PrevWindowHeight = *m_WindowHeight;
    }

    void Camera::ZoomIn(float x)
    {
        m_DistanceFromObject += x;
        if (m_DistanceFromObject < 1.5f)
            m_DistanceFromObject = 1.5f;
        else if (m_DistanceFromObject > 9.0f)
            m_DistanceFromObject = 9.0f;
    }

    void Camera::AddPitch(float p)
    {
        m_Rotation.x += p;
    }

    void Camera::AddYaw(float yaw)
    {
        m_AngleAroundPlayer += yaw;
    }

    glm::vec3 Camera::GetPosition()
    {
        return m_Position;
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        return m_ViewMatrix;
    }

    glm::mat4 Camera::GetProjectiomMatrix()
    {
        return m_ProjectionMatrix;
    }

    float Camera::GetFarPlane()
    {
        return m_FarPlane;
    }

    unsigned int Camera::GetWindowWidth()
    {
        return *m_WindowWidth;
    }

    unsigned int Camera::GetWindowHeight()
    {
        return *m_WindowHeight;
    }

    void Camera::CreateViewMatrix(bool overrideUpdateCall)
    {
        glm::mat4 basicMatrix;
        glm::mat4 rotX = glm::rotate(basicMatrix, m_Rotation.x, glm::vec3(1, 0, 0));
        glm::mat4 rotY = glm::rotate(rotX, m_Rotation.y, glm::vec3(0, 1, 0));
        glm::mat4 rotZ = glm::rotate(rotY, m_Rotation.z, glm::vec3(0, 0, 1));
        glm::vec3 negPos = glm::vec3(-m_Position.x, -m_Position.y, -m_Position.z);
        m_ViewMatrix = glm::translate(rotZ, negPos);
    }

    void Camera::CreateProjectionMatrix(bool overrideUpdateCall)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
    }

    void Camera::CalculatePosition(float horizontalDistance, float verticalDistance)
    {
        if (m_Model != nullptr)
        {
            float theta = m_AngleAroundPlayer;
            float offsetX = horizontalDistance * sinf(theta);
            float offsetZ = horizontalDistance * cosf(theta);
            m_Rotation.y = PI - m_AngleAroundPlayer;
            m_Position.x = m_Model->GetPosition().x - offsetX;
            m_Position.y = m_Model->GetPosition().y + verticalDistance;
            m_Position.z = m_Model->GetPosition().z - offsetZ;
            if (!m_bPlayerAttached)
            {
                AddPitch(PI / 4);
                m_bPlayerAttached = true;
            }
        }
    }

    float Camera::CalculateVertDistance()
    {
        return m_DistanceFromObject * sinf(m_Rotation.x);
    }

    float Camera::CalculateHorisDistance()
    {
        return m_DistanceFromObject * cosf(m_Rotation.x);
    }

}