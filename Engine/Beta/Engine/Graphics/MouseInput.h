#pragma once
#include "InputDevice.h"
#include "Camera.h"

namespace Graphics
{
	/*Tracks the user input from a mouse.
	For 3D tracking, a camera must be attatched*/
	class MouseInput:
		public InputDevice
	{
	public:
		GRAPHICS_DLL_API virtual glm::vec2 GetPositon()final;
		GRAPHICS_DLL_API virtual glm::vec3 Get3DPosition()final;
		GRAPHICS_DLL_API virtual glm::vec3 Get3DPosition_2()final;
		GRAPHICS_DLL_API virtual void AttatchCamera(std::shared_ptr<Graphics::Camera>& camera)final;

	protected:
		friend class GraphicsWindow;
		GRAPHICS_DLL_API virtual void SetPosition(float x, float y);

	private:
		glm::vec2 GetTranslatedCoords();
		glm::vec4 GetEyeSpaceCoords(glm::vec4 position);
		glm::vec3 GetWorldSpaceCoords();

		glm::vec2 m_position;
		std::mutex m_mouse_lock;
		std::shared_ptr<Graphics::Camera> m_camera;
		std::unordered_map<int, bool> m_KeysDown;
		std::unordered_map<int, bool> m_KeysPressed;
	};
}


