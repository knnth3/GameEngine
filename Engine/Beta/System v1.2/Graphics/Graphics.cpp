#include "Graphics.h"
#include "DirectX11API.h"
#include "RenderBatch.h"


Graphics::GraphicsDevice::GraphicsDevice(std::shared_ptr<Camera> camera)
{
	m_camera = camera;
	m_graphicsAPI = std::make_unique<DirectX11API>();
}

Graphics::GraphicsDevice::~GraphicsDevice()
{
}

bool Graphics::GraphicsDevice::Initialize(void * handle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, bool fullscreen)
{
	bool result = false;
	if (m_camera)
	{
		result = m_graphicsAPI->Initialize(handle, screenWidth, screenHeight, vsync, fullscreen);
		if (result)
		{
			m_renderBatch = std::make_unique<DX11Graphics>(m_graphicsAPI->GetBufferManager(),
				m_graphicsAPI->GetShaderManager(), m_graphicsAPI->GetDepthStencilState(),
				m_graphicsAPI->GetRasterStateManager());
			m_renderBatch->Initialize(m_camera);
		}
	}
	return result;
}

void Graphics::GraphicsDevice::BeginScene(float r, float g, float b)
{
	ClearScreen(r, g, b);
	BeginScene();
}

void Graphics::GraphicsDevice::BeginScene()
{
	m_graphicsAPI->BeginScene();
}

void Graphics::GraphicsDevice::Draw(std::shared_ptr<Model> model)
{
	//Major optimization: Use unique id's(uint16_t == 65k == way more models on screen than we can ever dream to handle) to get info
	//No need for ptrs as they have overhead
	m_renderBatch->AddModel(model);
}

void Graphics::GraphicsDevice::EndScene()
{
	m_renderBatch->Draw();
	m_graphicsAPI->EndScene();
}

void Graphics::GraphicsDevice::Close()
{
	m_graphicsAPI->Close();
}

void Graphics::GraphicsDevice::SetWindowSize(uint16_t width, uint16_t height)
{
	m_graphicsAPI->SetWindowSize(width, height);
}

void Graphics::GraphicsDevice::GetVideoCardInfo(std::string& name, int& memory)
{
	m_graphicsAPI->GetVideoCardInfo(name, memory);
}

void Graphics::GraphicsDevice::ClearScreen(float r, float g, float b)
{
	m_graphicsAPI->ClearScreen(r, g, b);
}

void Graphics::GraphicsDevice::DrawBackToFront(bool val)
{
	m_renderBatch->DrawBackToFront(val);
}
