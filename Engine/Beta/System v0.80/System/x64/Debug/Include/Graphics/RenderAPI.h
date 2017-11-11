#pragma once
#include "DllSettings.h"
#include "Camera.h"


namespace Graphics
{
	class RenderAPI
	{
	public:
		virtual bool Initialize(void* handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen) = 0;
		virtual void BeginScene() = 0;
		virtual void EndScene() = 0;
		virtual void Close() = 0;
		virtual void GetVideoCardInfo(std::string& name, int& memory) = 0;
		virtual void ClearScreen(float r, float g, float b) = 0;
		//virtual std::shared_ptr<BufferManager> GetBufferManager() = 0;
		//virtual std::shared_ptr<ShaderManager> GetShaderManager() = 0;
		virtual void AddModel(std::shared_ptr<Model::Model3D>& model) = 0;
		virtual void AttatchCamera(std::shared_ptr<Camera>& ptr) = 0;
	};
}


