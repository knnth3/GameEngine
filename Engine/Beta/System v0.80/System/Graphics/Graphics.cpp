#include "Graphics.h"
#include "DX11Graphics.h"


Graphics::GraphicsDevice::GraphicsDevice(std::shared_ptr<Camera> camera)
{
	m_camera = camera;
	m_graphicsAPI = std::make_unique<DX11Graphics>();
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
			m_graphicsAPI->AttatchCamera(m_camera);
		}
		//if (result)
		//	m_renderBatch = std::make_unique<RenderBatch>(m_graphicsAPI->GetBufferManager(), m_graphicsAPI->GetShaderManager(), m_camera);
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

void Graphics::GraphicsDevice::Draw(std::shared_ptr<Model::Model3D>& model)
{
	m_graphicsAPI->AddModel(model);
	//m_renderBatch->AddModel(model);
}

void Graphics::GraphicsDevice::EndScene()
{
	//m_renderBatch->Draw();
	m_graphicsAPI->EndScene();
}

void Graphics::GraphicsDevice::Close()
{
	m_graphicsAPI->Close();
}

void Graphics::GraphicsDevice::GetVideoCardInfo(std::string& name, int& memory)
{
	m_graphicsAPI->GetVideoCardInfo(name, memory);
}

void Graphics::GraphicsDevice::ClearScreen(float r, float g, float b)
{
	m_graphicsAPI->ClearScreen(r, g, b);
}