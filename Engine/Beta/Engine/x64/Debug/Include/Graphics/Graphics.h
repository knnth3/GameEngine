#pragma once
#include "GraphicsResources.h"
#include "RenderBatch_2D.h"
#include "RenderBatch_3D.h"
namespace Graphics
{
	class GraphicsDevice
	{
	public:
		GRAPHICS_DLL_API GraphicsDevice();
		GRAPHICS_DLL_API GraphicsDevice(const CameraSettings s_camera);
		GRAPHICS_DLL_API ~GraphicsDevice();
		GRAPHICS_DLL_API virtual bool Initialize(HWND hwnd, DisplaySize size)final;
		GRAPHICS_DLL_API virtual void SetWindowDimensions(DisplaySize size)final;
		GRAPHICS_DLL_API virtual void SetClearColor(float r, float g, float b)final;
		GRAPHICS_DLL_API virtual void SetClearColor(glm::vec3 color)final;
		GRAPHICS_DLL_API virtual void BeginScene()final;
		GRAPHICS_DLL_API virtual void EndScene()final;
		GRAPHICS_DLL_API virtual void Present()final;
		GRAPHICS_DLL_API virtual void Trim()final;

		//Functionality
		GRAPHICS_DLL_API virtual std::shared_ptr<Camera> GetCamera()final;
		GRAPHICS_DLL_API virtual Graphics::DisplaySize GetWindowDimensions()final;
		GRAPHICS_DLL_API virtual void Draw(Model& model)final;
		GRAPHICS_DLL_API virtual void Draw(const Text& str);
		GRAPHICS_DLL_API virtual void Draw(const Square& sqr, bool background = false);
		GRAPHICS_DLL_API virtual void Draw(const Line& lne);
		GRAPHICS_DLL_API virtual void CreateDeviceDependentResources()final;
		GRAPHICS_DLL_API virtual void ReleaseDeviceDependentResources()final;
		GRAPHICS_DLL_API virtual void Wireframe(bool val)final;
		GRAPHICS_DLL_API virtual void Reset();
		GRAPHICS_DLL_API virtual void GetVideoCardInfo(std::string& name, int& memory)final;

	private:

		Graphics::DisplaySize m_size;
		glm::vec3 m_clearColor;
		std::shared_ptr<Camera> m_camera;
		std::unique_ptr<RenderBatch_3D> m_renderBatch_3D;
		std::unique_ptr<RenderBatch_2D> m_renderBatch_2D;
		std::unique_ptr<DeviceResources> m_deviceResources;
	};
}


