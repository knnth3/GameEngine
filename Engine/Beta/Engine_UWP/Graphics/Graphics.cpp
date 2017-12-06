#include "Graphics.h"
#include "RenderBatch.h"
#include <Graphics\TextureLoader.h>


Graphics::GraphicsDevice::GraphicsDevice()
{
	m_camera = std::shared_ptr<Camera>(new Camera());
}

Graphics::GraphicsDevice::GraphicsDevice(const CameraSettings s_camera)
{
	m_camera = std::shared_ptr<Camera>(new Camera(s_camera));
}

void Graphics::GraphicsDevice::BeginScene(float r, float g, float b)
{
	m_renderBatch_2D->BeginScene();
	this->ClearScreen(r, g, b);
}

bool Graphics::GraphicsDevice::Initialize(
	ID3D11Device3* device_3D, ID3D11DeviceContext3* context_3D,
	IDWriteFactory3* writeFactory, ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D)
{
	TextStyleLib::Initialize(writeFactory);
	bool result = false;
	m_constBuffer = std::shared_ptr<DX11ConstantBuffer>(new DX11ConstantBuffer(device_3D, context_3D));
	m_bufferManager = std::shared_ptr<DX11BufferManager>(new DX11BufferManager(device_3D, context_3D, m_constBuffer));
	m_shaderManager = std::shared_ptr<DX11ShaderManager>(new DX11ShaderManager(device_3D, context_3D));
	m_RSSManager = std::shared_ptr<DX11RasterStateManager>(new DX11RasterStateManager(device_3D, context_3D));
	m_renderBatch = std::unique_ptr<RenderBatch>(new RenderBatch(m_bufferManager, m_shaderManager, m_RSSManager));
	m_renderBatch_2D = std::unique_ptr<RenderBatch_2D>(new RenderBatch_2D(writeFactory, factory_2D, deviceContext_2D));

	result = m_renderBatch->Initialize(m_camera);
	if (result)
		result = TextureLoader::Initialize(device_3D, context_3D);

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
	m_renderBatch->AddModel(model);
}

void Graphics::GraphicsDevice::Draw(const Text & str)
{
	m_renderBatch_2D->Draw(str);
}

void Graphics::GraphicsDevice::Draw(const Square & sqr)
{
	m_renderBatch_2D->Draw(sqr);
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

void Graphics::GraphicsDevice::CreateNew2DBrush(std::string uniqueName, glm::vec4 color)
{
	m_renderBatch_2D->CreateNewBrush(uniqueName, color);
}

void Graphics::GraphicsDevice::Delete2DBrush(std::string uniqueName)
{
	m_renderBatch_2D->DeleteBrush(uniqueName);
}

void Graphics::GraphicsDevice::Wireframe(bool val)
{
	m_renderBatch->Wireframe(val);
}
