#pragma once
#include "RenderAPI.h"
#include "RenderBatch.h"

namespace Graphics
{
	class GraphicsDevice
	{
	public:
		DLL_API GraphicsDevice(std::shared_ptr<Camera> camera);
		DLL_API ~GraphicsDevice();
		DLL_API bool Initialize(void* handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen);
		DLL_API void BeginScene(float r, float g, float b);
		DLL_API void BeginScene();
		DLL_API void Draw(std::shared_ptr<Model::Model3D>& model);
		DLL_API void EndScene();
		DLL_API void Close();

		//Functionality
		DLL_API void GetVideoCardInfo(std::string& name, int& memory);
		DLL_API void ClearScreen(float r, float g, float b);

	private:
		std::shared_ptr<Camera> m_camera;
		std::unique_ptr<RenderAPI> m_graphicsAPI;
		std::unique_ptr<DX11Graphics> m_renderBatch;
		//std::unique_ptr<RenderBatch> m_renderBatch;
	};
}


