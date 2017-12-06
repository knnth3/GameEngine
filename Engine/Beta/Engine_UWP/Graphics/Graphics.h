#pragma once
#include "RenderBatch.h"
#include "RenderBatch_2D.h"

namespace Graphics
{
	class GraphicsDevice
	{
	protected:
		GRAPHICS_DLL_API virtual bool Initialize(
			ID3D11Device3* device_3D, ID3D11DeviceContext3* context_3D,
			IDWriteFactory3* writeFactory, 
			ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D, IWICImagingFactory2* wicFactory)final;

		GRAPHICS_DLL_API virtual void SetWindowDimensions(float x, float y)final;
		GRAPHICS_DLL_API virtual void Close()final;

	public:
		GRAPHICS_DLL_API GraphicsDevice();
		GRAPHICS_DLL_API GraphicsDevice(const CameraSettings s_camera);
		GRAPHICS_DLL_API virtual void BeginScene(float r, float g, float b)final;
		GRAPHICS_DLL_API virtual void EndScene()final;

		//Functionality
		GRAPHICS_DLL_API virtual std::shared_ptr<Camera> GetCamera()final;
		GRAPHICS_DLL_API virtual void Draw(Model& model)final;
		GRAPHICS_DLL_API virtual void Draw(const Text& str);
		GRAPHICS_DLL_API virtual void Draw(const Square& sqr);
		GRAPHICS_DLL_API virtual void Draw(const Line& lne);
		GRAPHICS_DLL_API virtual void CreateDeviceDependentResources()final;
		GRAPHICS_DLL_API virtual void ReleaseDeviceDependentResources()final;
		GRAPHICS_DLL_API virtual void GetWindowDimensions(float& x, float& y)final;
		GRAPHICS_DLL_API virtual void CreateNew2DBrush(std::string uniqueName, glm::vec4 color)final;
		GRAPHICS_DLL_API virtual void CreateNew2DBrush(std::string uniqueName, std::string filename)final;
		GRAPHICS_DLL_API virtual void Delete2DBrush(std::string uniqueName)final;
		GRAPHICS_DLL_API virtual void Wireframe(bool val)final;

		//Overridable
		GRAPHICS_DLL_API virtual void GetVideoCardInfo(std::string& name, int& memory);
		GRAPHICS_DLL_API virtual void ClearScreen(float r, float g, float b) = 0;

	private:

		float m_WindowWidth;
		float m_WindowHeight;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<DX11RasterStateManager> m_RSSManager;
		std::shared_ptr<DX11ShaderManager> m_shaderManager;
		std::shared_ptr<DX11BufferManager> m_bufferManager;
		std::shared_ptr<DX11ConstantBuffer> m_constBuffer;
		std::unique_ptr<RenderBatch> m_renderBatch;
		std::unique_ptr<RenderBatch_2D> m_renderBatch_2D;
	};
}


