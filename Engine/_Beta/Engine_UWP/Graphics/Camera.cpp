#include "Camera.h"

#define PI (float)M_PI

Graphics::Camera::Camera(CameraSettings defaultSettings)
{
	m_defaults = defaultSettings;
	Reset();
}

void Graphics::Camera::Reset()
{
	m_active = m_defaults;
	CalculateAspectRatio();
	Create3DProjectionMatrix();
}

void Graphics::Camera::SetDefaultSettings(CameraSettings info)
{
	m_defaults = info;
}

void Graphics::Camera::AttachToModel(const Model& model)
{
	m_active.m_focusPoint.m_position = model.GetPosition();
}

void Graphics::Camera::SetViewDistance(float nearPlane)
{
	m_active.m_nearDistance = nearPlane;
	Create3DProjectionMatrix();
}

void Graphics::Camera::SetFOV(float fov)
{
	m_active.m_fov = fov;
	Create3DProjectionMatrix();
}

void Graphics::Camera::Move(float x, float y, float z)
{
	float speed = 5.0f;
	m_active.m_position.x += x / speed;
	m_active.m_position.y += y / speed;
	m_active.m_position.z += z / speed;
}

void Graphics::Camera::SetPosition(glm::vec3 position)
{
	m_active.m_position = position;
}

void Graphics::Camera::Rotate(float x, float y, float z)
{
	float circle = (2.0f * PI);
	float xLimitUp = (PI / 2.0f);
	float xLimitDown = 0.0f;

	m_active.m_rotations.x += x;
	m_active.m_angleAroundPlayer -= y;

	if (m_bUsingBounds)
	{
		if (m_active.m_rotations.x > xLimitUp)
			m_active.m_rotations.x = xLimitUp;

		if (m_active.m_rotations.x < xLimitDown)
			m_active.m_rotations.x = xLimitDown;
	}

	if (m_active.m_angleAroundPlayer >= circle)
		m_active.m_angleAroundPlayer -= circle;
}

void Graphics::Camera::SetRotation(glm::vec3 rotation)
{
	SetAngeInBounds(rotation.x);
	m_active.m_rotations.x = rotation.x;

	SetAngeInBounds(rotation.y);
	m_active.m_angleAroundPlayer = rotation.y;

	SetAngeInBounds(rotation.z);
	m_active.m_rotations.z = rotation.z;
}

void Graphics::Camera::SetResolution(uint32_t width, uint32_t height)
{
	m_active.m_screenWidth = width;
	m_active.m_screenHeight = height;
	CalculateAspectRatio();
	Create3DProjectionMatrix();
}

void Graphics::Camera::Zoom(float x)
{
	float maxZoomOut = 700.0f;
	float maxZoomIn = 300.0f;

	m_active.m_distanceFromObject += x;

	if (m_bUsingBounds)
	{
		if (m_active.m_distanceFromObject < maxZoomIn)
			m_active.m_distanceFromObject = maxZoomIn;
		else if (m_active.m_distanceFromObject > maxZoomOut)
			m_active.m_distanceFromObject = maxZoomOut;
	}
}

glm::vec3 Graphics::Camera::GetPosition()
{
	return m_active.m_position;
}

glm::mat4 Graphics::Camera::GetViewMatrix()
{
	float horis = CalculateHorisDistance();
	float vert = CalculateVertDistance();
	CalculatePosition(horis, vert);
	CreateViewMatrix();
	return m_active.m_view;
}

glm::mat4 Graphics::Camera::GetIdentityMatrix()
{
	glm::mat4 identity(1.0f);
	return identity;
}

glm::mat4 Graphics::Camera::Get3DProjectionMatrix()
{
	return m_active.m_projection;
}

float Graphics::Camera::GetFarPlane()
{
	return m_active.m_farDistance;
}

unsigned int Graphics::Camera::GetWindowWidth()
{
	return m_active.m_screenWidth;
}

unsigned int Graphics::Camera::GetWindowHeight()
{
	return m_active.m_screenHeight;
}

void Graphics::Camera::EnforceBounds(bool val)
{
	m_bUsingBounds = val;
}

void Graphics::Camera::SetAngeInBounds(float & angle)
{
	float circle = (2.0f * PI);
	if (angle > circle)
	{
		int cycles = (int)(angle / circle);
		if (angle > 0.0f)
			angle = angle - (circle * (float)cycles);
		else
			angle = angle + (circle * (float)cycles);
	}
}

void Graphics::Camera::CalculateAspectRatio()
{
	float xResolution = (float)m_active.m_screenWidth;
	float yResolution = (float)m_active.m_screenHeight;
	m_active.m_aspectRatio = xResolution / yResolution;
}

void Graphics::Camera::CreateViewMatrix()
{
	glm::mat4 basicMatrix;
	glm::mat4 rotX = glm::rotate(basicMatrix, m_active.m_rotations.x, glm::vec3(1, 0, 0));
	glm::mat4 rotY = glm::rotate(rotX, m_active.m_rotations.y, glm::vec3(0, 1, 0));
	glm::mat4 rotZ = glm::rotate(rotY, m_active.m_rotations.z, glm::vec3(0, 0, 1));
	glm::vec3 negPos = m_active.m_position * -1.0f;
	m_active.m_view = glm::translate(rotZ, negPos);
}

void Graphics::Camera::Create3DProjectionMatrix()
{
	m_active.m_projection = glm::perspectiveRH(m_active.m_fov, m_active.m_aspectRatio, m_active.m_nearDistance, m_active.m_farDistance);
}

void Graphics::Camera::CalculatePosition(float horizontalDistance, float verticalDistance)
{
	if (!m_active.m_focusPoint.Empty())
	{
		//Raise the camera to float half a meter in the air
		glm::vec3 charPos = m_active.m_focusPoint.GetPosition();
		charPos.y += m_active.m_cameraHeight;

		float theta = m_active.m_angleAroundPlayer;
		float offsetX = horizontalDistance * sinf(theta);
		float offsetZ = horizontalDistance * cosf(theta);
		m_active.m_rotations.y = PI - m_active.m_angleAroundPlayer;
		m_active.m_position.x = charPos.x - offsetX;
		m_active.m_position.y = charPos.y + verticalDistance;
		m_active.m_position.z = charPos.z - offsetZ;
	}
}

void Graphics::Camera::CalculateClippingPlanes()
{
	float hNear = 2.0f * tanf(m_active.m_fov / 2.0f) * m_active.m_nearDistance;
	float wNear = hNear * m_active.m_aspectRatio;

	float Hfar = 2.0f * tanf(m_active.m_fov / 2.0f) * m_active.m_farDistance;
	float Wfar = Hfar * m_active.m_aspectRatio;
}

float Graphics::Camera::CalculateVertDistance()
{
	return m_active.m_distanceFromObject * sinf(m_active.m_rotations.x);
}

float Graphics::Camera::CalculateHorisDistance()
{
	return m_active.m_distanceFromObject * cosf(m_active.m_rotations.x);
}

Graphics::CameraSettings::CameraSettings()
{
	m_fov = 0.25f* 3.14f;
	m_aspectRatio = 0.0f;
	m_nearDistance = 1.0f;
	m_farDistance = 5000.0f;
	m_distanceFromObject = 600.0f;
	m_angleAroundPlayer = PI;
	m_cameraHeight = METER * 0.5f;
	m_view = glm::mat4(0.0f);
	m_projection = glm::mat4(0.0f);
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	m_rotations = glm::vec3(PI / 4.0f, 0.0f, 0.0f);
	m_screenWidth = 800;
	m_screenHeight = 600;
}

Graphics::Position::Position()
{
	m_position = vec3_ptr(new glm::vec3(0.0f, 0.0f, 0.0f));
}

bool Graphics::Position::Empty()
{
	if (m_position == nullptr)
		return true;

	return false;
}

glm::vec3 Graphics::Position::GetPosition()
{
	return *m_position;
}
