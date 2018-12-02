#pragma once
#include "DirectXResources.h"
#include "RenderBatch_2D.h"
#include "RenderBatch_3D.h"
namespace Engine
{
	typedef std::shared_ptr<Engine::Camera> ECamera;
	typedef std::shared_ptr<TextureLibrary> ETextureLib;
	class GraphicsDevice
	{
	public:
		__engine_decl GraphicsDevice();
		__engine_decl GraphicsDevice(const CameraSettings s_camera);
		__engine_decl ~GraphicsDevice();
		__engine_decl virtual bool Initialize(HWND hwnd, DisplaySize size)final;
		__engine_decl virtual void SetWindowDimensions(DisplaySize size)final;
		__engine_decl virtual void SetClearColor(float r, float g, float b)final;
		__engine_decl virtual void SetClearColor(glm::vec3 color)final;
		__engine_decl virtual void BeginScene()final;
		__engine_decl virtual void EndScene()final;
		__engine_decl virtual void Present()final;
		__engine_decl virtual void Trim()final;

		//Functionality
		__engine_decl virtual ECamera GetCamera()final;
		__engine_decl virtual ETextureLib GetTextureLibrary()final;
		__engine_decl virtual Engine::DisplaySize GetWindowDimensions()final;
		__engine_decl virtual void Draw(const Model& model)final;
		__engine_decl virtual void Draw(const Text& str);
		__engine_decl virtual void Draw(const Square& sqr, bool background = false);
		__engine_decl virtual void Draw(const Line& lne);
		__engine_decl virtual void CreateDeviceDependentResources()final;
		__engine_decl virtual void ReleaseDeviceDependentResources()final;
		__engine_decl virtual void Wireframe(bool val)final;
		__engine_decl virtual void ToggleWireframe()final;
		__engine_decl virtual void Reset();
		virtual void SetSkybox(Skybox& skybox);
		__engine_decl virtual std::vector<VideoCardInfo> GetVideoCardInfo()const final;

	private:

		Engine::DisplaySize m_size;
		glm::vec3 m_clearColor;
		std::shared_ptr<Camera> m_camera;
		std::unique_ptr<RenderBatch_3D> m_renderBatch_3D;
		std::unique_ptr<RenderBatch_2D> m_renderBatch_2D;
		std::unique_ptr<DeviceResources> m_deviceResources;
	};
}


