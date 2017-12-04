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
			IDWriteFactory3* writeFactory, ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D)final;

		GRAPHICS_DLL_API virtual void ResizeSpriteBatch_2D(float x, float y)final;
	public:
		GRAPHICS_DLL_API GraphicsDevice();
		GRAPHICS_DLL_API GraphicsDevice(const CameraSettings s_camera);
		GRAPHICS_DLL_API virtual void EndScene()final;

		//Functionality
		GRAPHICS_DLL_API virtual std::shared_ptr<Camera> GetCamera()final;
		GRAPHICS_DLL_API virtual void DrawBackToFront(bool val)final;
		GRAPHICS_DLL_API virtual void Draw(Model& model)final;
		GRAPHICS_DLL_API virtual void Draw(const Text& str);
		GRAPHICS_DLL_API virtual void CreateDeviceDependentResources()final;
		GRAPHICS_DLL_API virtual void ReleaseDeviceDependentResources()final;
		
		//Overridable
		GRAPHICS_DLL_API virtual void GetVideoCardInfo(std::string& name, int& memory);
		GRAPHICS_DLL_API virtual void BeginScene(float r, float g, float b);

	private:

		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<DX11RasterStateManager> m_RSSManager;
		std::shared_ptr<DX11ShaderManager> m_shaderManager;
		std::shared_ptr<DX11BufferManager> m_bufferManager;
		std::shared_ptr<DX11ConstantBuffer> m_constBuffer;
		std::unique_ptr<RenderBatch> m_renderBatch;
		std::unique_ptr<RenderBatch_2D> m_renderBatch_2D;
	};
}


