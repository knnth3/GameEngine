#pragma once
#include "RenderAPI.h"
#include "RenderBatch.h"

namespace Graphics
{
	class GraphicsDevice
	{
	public:
		GRAPHICS_DLL_API GraphicsDevice(std::shared_ptr<Camera> camera);
		GRAPHICS_DLL_API ~GraphicsDevice();
		GRAPHICS_DLL_API bool Initialize(void* handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen);
		GRAPHICS_DLL_API void BeginScene(float r, float g, float b);
		GRAPHICS_DLL_API void BeginScene();
		GRAPHICS_DLL_API void Draw(Model& model);
		GRAPHICS_DLL_API void EndScene();
		GRAPHICS_DLL_API void Close();

		//Functionality
		GRAPHICS_DLL_API void SetWindowSize(uint16_t  width, uint16_t  height);
		GRAPHICS_DLL_API void GetVideoCardInfo(std::string& name, int& memory);
		GRAPHICS_DLL_API void ClearScreen(float r, float g, float b);
		GRAPHICS_DLL_API void DrawBackToFront(bool val);

	private:
		std::shared_ptr<Camera> m_camera;
		std::unique_ptr<RenderAPI> m_graphicsAPI;
		std::unique_ptr<DX11Graphics> m_renderBatch;
	};
}


