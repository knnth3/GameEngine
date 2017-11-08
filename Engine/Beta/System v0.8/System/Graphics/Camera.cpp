#include "Camera.h"

#define PI (float)M_PI

Graphics::Camera::Camera(CameraSettings defaultSettings)
{
	m_currentSettings = defaultSettings;
	CalculateAspectRatio();
	Create3DProjectionMatrix();
	Create2DProjectionMatrix();
}

void Graphics::Camera::Reset()
{
	m_currentSettings = m_defaultSettings;
}

void Graphics::Camera::SetDefaultSettings(CameraSettings info)
{
	m_defaultSettings = info;
}

void Graphics::Camera::AttachToModel(std::shared_ptr<Model::Model3D>& model)
{
	m_currentSettings.m_model = model;
}

void Graphics::Camera::SetViewDistance(float nearPlane)
{
	m_currentSettings.m_nearPlane = nearPlane;
	Create3DProjectionMatrix();
	Create2DProjectionMatrix();
}

void Graphics::Camera::SetFOV(float fov)
{
	m_currentSettings.m_fov = fov;
	Create3DProjectionMatrix();
	Create2DProjectionMatrix();
}

void Graphics::Camera::Move(float x, float y, float z)
{
	float speed = 5.0f;
	m_currentSettings.m_position.x += x / speed;
	m_currentSettings.m_position.y += y / speed;
	m_currentSettings.m_position.z += z / speed;
}

void Graphics::Camera::SetPosition(glm::vec3 position)
{
	m_currentSettings.m_position = position;
}

void Graphics::Camera::Rotate(float x, float y, float z)
{
	float circle = (2.0f * PI);
	float xLimitUp = (PI / 2.0f);
	float xLimitDown = 0.0f;

	m_currentSettings.m_angleAroundPlayer -= x;
	m_currentSettings.m_rotation.x += y;

	if (m_currentSettings.m_rotation.x > xLimitUp)
		m_currentSettings.m_rotation.x = xLimitUp;

	if (m_currentSettings.m_rotation.x < xLimitDown)
		m_currentSettings.m_rotation.x = xLimitDown;

	if (m_currentSettings.m_angleAroundPlayer >= circle)
		m_currentSettings.m_angleAroundPlayer -= circle;
}

void Graphics::Camera::SetRotation(glm::vec3 rotation)
{
	float circle = (2.0f * PI);
	if (rotation.x >= circle)
		rotation.x -= circle;

	if (rotation.y >= circle)
		rotation.y -= circle;

	if (rotation.z >= circle)
		rotation.z -= circle;
	m_currentSettings.m_rotation = rotation;
}

void Graphics::Camera::SetResolution(uint32_t width, uint32_t height)
{
	m_currentSettings.m_xResolution = width;
	m_currentSettings.m_yResolution = height;
	CalculateAspectRatio();
	Create3DProjectionMatrix();
	Create2DProjectionMatrix();
}

void Graphics::Camera::Zoom(float x)
{
	float maxZoomOut = 700.0f;
	float maxZoomIn = 300.0f;

	m_currentSettings.m_distanceFromObject += x;
	if (m_currentSettings.m_distanceFromObject < maxZoomIn)
		m_currentSettings.m_distanceFromObject = maxZoomIn;
	else if (m_currentSettings.m_distanceFromObject > maxZoomOut)
		m_currentSettings.m_distanceFromObject = maxZoomOut;
}

glm::vec3 Graphics::Camera::GetPosition()
{
	return m_currentSettings.m_position;
}

glm::mat4 Graphics::Camera::GetViewMatrix()
{
	float horis = CalculateHorisDistance();
	float vert = CalculateVertDistance();
	CalculatePosition(horis, vert);
	CreateViewMatrix();
	return m_currentSettings.m_view;
}

glm::mat4 Graphics::Camera::GetIdentityMatrix()
{
	glm::mat4 identity(1.0f);
	return identity;
}

glm::mat4 Graphics::Camera::Get3DProjectionMatrix()
{
	return m_currentSettings.m_3Dprojection;
}

glm::mat4 Graphics::Camera::Get2DProjectionMatrix()
{
	return m_currentSettings.m_2Dprojection;
}

float Graphics::Camera::GetFarPlane()
{
	return m_currentSettings.m_farPlane;
}

unsigned int Graphics::Camera::GetWindowWidth()
{
	return m_currentSettings.m_xResolution;
}

unsigned int Graphics::Camera::GetWindowHeight()
{
	return m_currentSettings.m_yResolution;
}

void Graphics::Camera::CalculateAspectRatio()
{
	float xResolution = (float)m_currentSettings.m_xResolution;
	float yResolution = (float)m_currentSettings.m_yResolution;
	m_currentSettings.m_aspectRatio = xResolution / yResolution;
}

void Graphics::Camera::CreateViewMatrix()
{
	glm::mat4 basicMatrix;
	glm::mat4 rotX = glm::rotate(basicMatrix, m_currentSettings.m_rotation.x, glm::vec3(1, 0, 0));
	glm::mat4 rotY = glm::rotate(rotX, m_currentSettings.m_rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 rotZ = glm::rotate(rotY, m_currentSettings.m_rotation.z, glm::vec3(0, 0, 1));
	glm::vec3 negPos = m_currentSettings.m_position * -1.0f;
	m_currentSettings.m_view = glm::translate(rotZ, negPos);
}

void Graphics::Camera::Create3DProjectionMatrix()
{
	m_currentSettings.m_3Dprojection = glm::infinitePerspective(m_currentSettings.m_fov, m_currentSettings.m_aspectRatio, m_currentSettings.m_nearPlane);
}

void Graphics::Camera::Create2DProjectionMatrix()
{
	float top = 0.0f;
	float left = 0.0f;
	float bottom = ((float)m_currentSettings.m_yResolution * -1.0f);
	float right = (float)m_currentSettings.m_xResolution;
	m_currentSettings.m_2Dprojection = glm::ortho(left, right, bottom, top, m_currentSettings.m_nearPlane, m_currentSettings.m_farPlane);
}

void Graphics::Camera::CalculatePosition(float horizontalDistance, float verticalDistance)
{
	if (m_currentSettings.m_model != nullptr)
	{
		//Raise the camera to float half a meter in the air
		glm::vec3 charPos = m_currentSettings.m_model->GetPosition();
		charPos.y += m_currentSettings.m_cameraHeight;

		float theta = m_currentSettings.m_angleAroundPlayer;
		float offsetX = horizontalDistance * sinf(theta);
		float offsetZ = horizontalDistance * cosf(theta);
		m_currentSettings.m_rotation.y = PI - m_currentSettings.m_angleAroundPlayer;
		m_currentSettings.m_position.x = charPos.x - offsetX;
		m_currentSettings.m_position.y = charPos.y + verticalDistance;
		m_currentSettings.m_position.z = charPos.z - offsetZ;
	}
}

float Graphics::Camera::CalculateVertDistance()
{
	return m_currentSettings.m_distanceFromObject * sinf(m_currentSettings.m_rotation.x);
}

float Graphics::Camera::CalculateHorisDistance()
{
	return m_currentSettings.m_distanceFromObject * cosf(m_currentSettings.m_rotation.x);
}

Graphics::CameraSettings::CameraSettings()
{
	m_fov = 0.25f* 3.14f;
	m_aspectRatio = 0.0f;
	m_nearPlane = 1.0f;
	m_distanceFromObject = 600.0f;
	m_angleAroundPlayer = 0.0f;
	m_cameraHeight = METER * 0.5f;
	m_view = glm::mat4(0.0f);
	m_3Dprojection = glm::mat4(0.0f);
	m_2Dprojection = glm::mat4(0.0f);
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	m_rotation = glm::vec3(PI / 4.0f, 0.0f, 0.0f);
	m_xResolution = 800;
	m_yResolution = 600;
	m_model = nullptr;
}
