#include "Graphics.h"
#include "RenderBatch.h"
#include <Graphics\TextureLoader.h>

#define ENABLE_3D_RENDERING true

Graphics::GraphicsDevice::GraphicsDevice()
{
	m_camera = std::shared_ptr<Camera>(new Camera());
	clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
}

Graphics::GraphicsDevice::GraphicsDevice(const CameraSettings s_camera)
{
	m_camera = std::shared_ptr<Camera>(new Camera(s_camera));
}

void Graphics::GraphicsDevice::SetClearColor(float r, float g, float b)
{
	clearColor.r = r;
	clearColor.g = g;
	clearColor.b = b;
}

void Graphics::GraphicsDevice::SetClearColor(glm::vec3 color)
{
	clearColor = color;
}

void Graphics::GraphicsDevice::BeginScene()
{
	float alpha = 1.0f;
	m_renderBatch_2D->BeginScene();

	if (ENABLE_3D_RENDERING)
	{
		ClearScreen(clearColor.r, clearColor.g, clearColor.b);
		alpha = 0.0f;
	}

	m_renderBatch_2D->ClearScreen(clearColor.r, clearColor.g, clearColor.b, alpha);
}

bool Graphics::GraphicsDevice::Initialize(
	ID3D11Device3* device_3D, ID3D11DeviceContext3* context_3D,
	IDWriteFactory3* writeFactory,
	ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D, IWICImagingFactory2* wicFactory)
{
	TextStyleLib::Initialize(writeFactory);
	bool result = false;
	if (ENABLE_3D_RENDERING)
	{
		m_constBuffer = std::shared_ptr<DX11ConstantBuffer>(new DX11ConstantBuffer(device_3D, context_3D));
		m_bufferManager = std::shared_ptr<DX11BufferManager>(new DX11BufferManager(device_3D, context_3D, m_constBuffer));
		m_shaderManager = std::shared_ptr<DX11ShaderManager>(new DX11ShaderManager(device_3D, context_3D));
		m_RSSManager = std::shared_ptr<DX11RasterStateManager>(new DX11RasterStateManager(device_3D, context_3D));
		m_renderBatch = std::unique_ptr<RenderBatch>(new RenderBatch(m_bufferManager, m_shaderManager, m_RSSManager));

		result = m_renderBatch->Initialize(m_camera);
		if (result)
			result = TextureLoader::Initialize(device_3D, context_3D);
	}

	m_renderBatch_2D = std::unique_ptr<RenderBatch_2D>(new RenderBatch_2D(writeFactory, factory_2D, deviceContext_2D, wicFactory));

	m_renderBatch_2D->Initialize();
	return result;
}

void Graphics::GraphicsDevice::SetWindowDimensions(float x, float y)
{
	m_WindowWidth = x;
	m_WindowHeight = y;
	m_renderBatch_2D->SetDimensions(x, y);
}

void Graphics::GraphicsDevice::Close()
{
	TextStyleLib::Close();
}

void Graphics::GraphicsDevice::EndScene()
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch->Draw();

	m_renderBatch_2D->EndScene();
}

std::shared_ptr<Graphics::Camera> Graphics::GraphicsDevice::GetCamera()
{
	return m_camera;
}

void Graphics::GraphicsDevice::GetVideoCardInfo(std::string& name, int& memory)
{
}

void Graphics::GraphicsDevice::Draw(Model& model)
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch->AddModel(model);
}

void Graphics::GraphicsDevice::Draw(const Text & str)
{
	m_renderBatch_2D->Draw(str);
}

void Graphics::GraphicsDevice::Draw(const Square & sqr, bool background)
{
	m_renderBatch_2D->Draw(sqr, background);
}

void Graphics::GraphicsDevice::Draw(const Line & lne)
{
	m_renderBatch_2D->Draw(lne);
}

void Graphics::GraphicsDevice::CreateDeviceDependentResources()
{
	m_renderBatch_2D->CreateDeviceDependentResources();
}

void Graphics::GraphicsDevice::ReleaseDeviceDependentResources()
{
	m_renderBatch_2D->ReleaseDeviceDependentResources();
}

void Graphics::GraphicsDevice::GetWindowDimensions(float & x, float & y)
{
	x = m_WindowWidth;
	y = m_WindowHeight;
}

void Graphics::GraphicsDevice::Wireframe(bool val)
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch->Wireframe(val);
}

void Graphics::GraphicsDevice::Reset()
{
	if (ENABLE_3D_RENDERING)
	{
		m_renderBatch->Reset();
		m_camera->Reset();
		m_camera->SetResolution(m_WindowWidth, m_WindowHeight);
	}
}
