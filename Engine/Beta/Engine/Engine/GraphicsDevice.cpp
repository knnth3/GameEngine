#include "GraphicsDevice.h"

#define ENABLE_3D_RENDERING true

Engine::GraphicsDevice::GraphicsDevice() :
	GraphicsDevice(CameraSettings())
{
}

Engine::GraphicsDevice::GraphicsDevice(const CameraSettings s_camera)
{
	m_camera = std::shared_ptr<Camera>(new Camera(s_camera));
	m_clearColor = glm::vec3(0.2f, 0.2f, 0.2f);
}

Engine::GraphicsDevice::~GraphicsDevice()
{
	TextStyleLib::Close();
}

void Engine::GraphicsDevice::SetClearColor(float r, float g, float b)
{
	m_clearColor.r = r;
	m_clearColor.g = g;
	m_clearColor.b = b;
}

void Engine::GraphicsDevice::SetClearColor(glm::vec3 color)
{
	m_clearColor = color;
}

void Engine::GraphicsDevice::BeginScene()
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

bool Engine::GraphicsDevice::Initialize(HWND hwnd, DisplaySize size)
{
	bool result = true;
	m_size = size;
	m_deviceResources = std::make_unique<DeviceResources>();
	m_deviceResources->SetHandleInfo(hwnd, m_size);

	auto writeFactory = m_deviceResources->GetDWriteFactory();
	auto factory_2D = m_deviceResources->GetD2DFactory();
	auto context_2D = m_deviceResources->GetD2DDeviceContext();
	auto wicFactory = m_deviceResources->GetWicImagingFactory();

	if (!TextStyleLib::Initialize(writeFactory))
	{
		OpenDialog(L"Text style library", L"Failed to initialize.");
		result = false;
	}
	if (ENABLE_3D_RENDERING && result)
	{
		auto device_3D = m_deviceResources->GetD3DDevice();
		auto context_3D = m_deviceResources->GetD3DDeviceContext();

		m_renderBatch_3D = std::unique_ptr<RenderBatch_3D>(new RenderBatch_3D(device_3D, context_3D));
		if (!m_renderBatch_3D->Initialize(m_camera))
			result = false;
	}
	if (result)
	{
		m_renderBatch_2D = std::unique_ptr<RenderBatch_2D>(new RenderBatch_2D(writeFactory, factory_2D, context_2D, wicFactory));
		m_renderBatch_2D->Initialize();
	}
	else
	{
		OpenDialog(L"Graphics Engine", L"Some essential assets failed to load. Requesting program exit.");
	}

	return result;
}

void Engine::GraphicsDevice::SetWindowDimensions(DisplaySize size)
{
	m_size = size;
	m_deviceResources->SetLogicalSize(m_size);
	m_camera->SetResolution(m_size.Width, m_size.Height);
	m_renderBatch_2D->SetDimensions(m_size.Width, m_size.Height);
}

void Engine::GraphicsDevice::EndScene()
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch_3D->ProcessScene();

	m_renderBatch_2D->EndScene();
}

void Engine::GraphicsDevice::Present()
{
	m_deviceResources->Present();
}

void Engine::GraphicsDevice::Trim()
{
	m_deviceResources->Trim();
}

std::shared_ptr<Engine::Camera> Engine::GraphicsDevice::GetCamera()
{
	return m_camera;
}

std::shared_ptr<Engine::TextureLibrary> Engine::GraphicsDevice::GetTextureLibrary()
{
	return m_renderBatch_3D->GetTextureLibrary();
}

void Engine::GraphicsDevice::Draw(Model& model)
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch_3D->Draw(model);
}

void Engine::GraphicsDevice::Draw(const Text & str)
{
	m_renderBatch_2D->Draw(str);
}

void Engine::GraphicsDevice::Draw(const Square & sqr, bool background)
{
	m_renderBatch_2D->Draw(sqr, background);
}

void Engine::GraphicsDevice::Draw(const Line & lne)
{
	m_renderBatch_2D->Draw(lne);
}

void Engine::GraphicsDevice::CreateDeviceDependentResources()
{
	m_renderBatch_2D->CreateDeviceDependentResources();
}

void Engine::GraphicsDevice::ReleaseDeviceDependentResources()
{
	m_renderBatch_2D->ReleaseDeviceDependentResources();
}

Engine::DisplaySize Engine::GraphicsDevice::GetWindowDimensions()
{
	return m_size;
}

void Engine::GraphicsDevice::Wireframe(bool val)
{
	if(ENABLE_3D_RENDERING)
		m_renderBatch_3D->Wireframe(val);
}

void Engine::GraphicsDevice::ToggleWireframe()
{
	if (ENABLE_3D_RENDERING)
		m_renderBatch_3D->ToggleWireframe();
}

void Engine::GraphicsDevice::Reset()
{
	if (ENABLE_3D_RENDERING)
	{
		m_renderBatch_3D->Reset();
		m_camera->Reset();
		m_camera->SetResolution(m_size.Width, m_size.Height);
	}
}

void Engine::GraphicsDevice::SetSkybox(Skybox & skybox)
{
	auto textureLib = m_renderBatch_3D->GetTextureLibrary();
	textureLib->CreateTexture("CUBEMAP", skybox.path);
}

std::vector<Engine::VideoCardInfo> Engine::GraphicsDevice::GetVideoCardInfo() const
{
	return m_deviceResources->GetVideoCardInfo();
}
