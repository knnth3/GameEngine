#include "Graphics.h"

#define ENABLE_3D_RENDERING true

Graphics::GraphicsDevice::GraphicsDevice()
{
	m_camera = std::shared_ptr<Camera>(new Camera());
	m_clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
}

Graphics::GraphicsDevice::GraphicsDevice(const CameraSettings s_camera)
{
	m_camera = std::shared_ptr<Camera>(new Camera(s_camera));
}

Graphics::GraphicsDevice::~GraphicsDevice()
{
	TextStyleLib::Close();
}

void Graphics::GraphicsDevice::SetClearColor(float r, float g, float b)
{
	m_clearColor.r = r;
	m_clearColor.g = g;
	m_clearColor.b = b;
}

void Graphics::GraphicsDevice::SetClearColor(glm::vec3 color)
{
	m_clearColor = color;
}

void Graphics::GraphicsDevice::BeginScene()
{
	float alpha = 1.0f;
	m_renderBatch_2D->BeginScene();

	if (ENABLE_3D_RENDERING)
	{
		m_deviceResources->ClearScreen(m_clearColor.r, m_clearColor.g, m_clearColor.b);
		alpha = 0.0f;
	}

	m_renderBatch_2D->ClearScreen(m_clearColor.r, m_clearColor.g, m_clearColor.b, alpha);
}

bool Graphics::GraphicsDevice::Initialize(HWND hwnd, DisplaySize size)
{
	m_size = size;
	m_deviceResources = std::make_unique<DeviceResources>();
	m_deviceResources->SetHandleInfo(hwnd, m_size);

	auto writeFactory = m_deviceResources->GetDWriteFactory();
	auto factory_2D = m_deviceResources->GetD2DFactory();
	auto context_2D = m_deviceResources->GetD2DDeviceContext();
	auto wicFactory = m_deviceResources->GetWicImagingFactory();
	TextStyleLib::Initialize(writeFactory);
	bool result = false;
	if (ENABLE_3D_RENDERING)
	{
		auto device_3D = m_deviceResources->GetD3DDevice();
		auto context_3D = m_deviceResources->GetD3DDeviceContext();
		//auto textureLoader = std::shared_ptr<TextureLoader>(new TextureLoader(device_3D, context_3D));
		//m_renderBatch = std::unique_ptr<RenderBatch>(new RenderBatch(bufferManager, shaderManager, RSSManager, textureLoader));
		//result = m_renderBatch->Initialize(m_camera);

		m_renderBatch_3D = std::unique_ptr<RenderBatch_3D>(new RenderBatch_3D(device_3D, context_3D));
		if (!m_renderBatch_3D->Initialize(m_camera))
			throw std::exception();
	}

	m_renderBatch_2D = std::unique_ptr<RenderBatch_2D>(new RenderBatch_2D(writeFactory, factory_2D, context_2D, wicFactory));
	m_renderBatch_2D->Initialize();
	return result;
}

void Graphics::GraphicsDevice::SetWindowDimensions(DisplaySize size)
{
	m_size = size;
	m_renderBatch_2D->SetDimensions(m_size.Width, m_size.Height);
}

void Graphics::GraphicsDevice::EndScene()
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch_3D->ProcessScene();

	m_renderBatch_2D->EndScene();
}

void Graphics::GraphicsDevice::Present()
{
	m_deviceResources->Present();
}

void Graphics::GraphicsDevice::Trim()
{
	m_deviceResources->Trim();
}

std::shared_ptr<Graphics::Camera> Graphics::GraphicsDevice::GetCamera()
{
	return m_camera;
}

std::shared_ptr<Graphics::TextureLibrary> Graphics::GraphicsDevice::GetTextureLibrary()
{
	return m_renderBatch_3D->GetTextureLibrary();
}

void Graphics::GraphicsDevice::Draw(Model& model)
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch_3D->Draw(model);
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

Graphics::DisplaySize Graphics::GraphicsDevice::GetWindowDimensions()
{
	return m_size;
}

void Graphics::GraphicsDevice::Wireframe(bool val)
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch_3D->Wireframe(val);
}

void Graphics::GraphicsDevice::Reset()
{
	if (ENABLE_3D_RENDERING)
	{
		m_renderBatch_3D->Reset();
		m_camera->Reset();
		m_camera->SetResolution(m_size.Width, m_size.Height);
	}
}

std::vector<Graphics::VideoCardInfo> Graphics::GraphicsDevice::GetVideoCardInfo() const
{
	return m_deviceResources->GetVideoCardInfo();
}
