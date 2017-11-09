#include "RenderBatch.h"
#include "MeshLoader.h"
#include "DX11TextureManager.h"

Graphics::DX11Graphics::DX11Graphics(std::shared_ptr<DX11BufferManager>& bufferManager, 
	std::shared_ptr<DX11ShaderManager> shaderManager,
	std::shared_ptr<DX11DepthStencilState>& depthStencilState):
	m_bufferCount(3)
{
	m_bFilled = false;
	m_bufferManager = bufferManager;
	m_shaderManager = shaderManager;
	m_depthStencilState = depthStencilState;
	m_vertexManager = std::make_shared<VertexManager>();
}

Graphics::DX11Graphics::~DX11Graphics()
{
}

bool Graphics::DX11Graphics::Initialize(std::shared_ptr<Camera>& camera)
{
	m_camera = camera;

	std::wstring vsPath = L"EngineAssets/shaders/VertexShader.hlsl";
	std::wstring psPath = L"EngineAssets/shaders/PixelShader.hlsl";
	m_shaderManager->CreateShader("3D", vsPath, psPath);

	CreateConstBuffers();

	return true;
}

void Graphics::DX11Graphics::AddModel(std::shared_ptr<Model::Model3D>& model)
{
	int ID = model->GetUniqueID();
	auto result = m_cachedModels_3D.find(ID);
	if (result == m_cachedModels_3D.end())
	{
		VertexManagerInfo info;
		info.model = model;
		m_vertexManager->AddModel(info);
		m_cachedModels_3D.emplace(ID, info);
	}
	m_modelDrawCache_3D.push_back(ID);
}

void Graphics::DX11Graphics::Draw()
{
	//Recompute if new data is available
	if (m_vertexManager->HasNewData())
		FillBuffers();

	//Draw objects
	if (m_bFilled)
	{
		//Process 3D objects
		for (auto x : m_modelDrawCache_3D)
		{
			m_bufferManager->SetAsActive(BufferTypes::BUFFER_3D);
			m_depthStencilState->SetDepthBufferStatus(true);
			ProcessObject_3D(m_cachedModels_3D[x]);
		}
		m_modelDrawCache_3D.clear();
	}
}

void Graphics::DX11Graphics::ProcessObject_3D(VertexManagerInfo& info)
{
	//Grab model info
	TextureID texture = info.model->GetTexture();
	MeshID mesh = info.model->GetMesh();

	//Activate relevant texture
	DX11TextureManager::SetDefaultActive(DefaultTextures::MODEL);
	if (!m_shaderManager->SetActive("3D"))
		return;

	//Add info Here
	MatrixBuffer matrices;
	PF_PixelBuffer lightInfo;
	matrices.world = glm::transpose(info.model->GetModelMatrix());
	matrices.view = glm::transpose(m_camera->GetViewMatrix());
	matrices.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
	matrices.cameraPos = m_camera->GetPosition();
	matrices.color = info.model->GetColor();
	m_bufferManager->SetConstantBufferData("Matrix", &matrices, Graphics::ShaderType::Vertex);

	lightInfo.ambientColor = m_light.m_ambientColor;
	lightInfo.diffuseColor = m_light.m_diffuseColor;
	lightInfo.lightDirection = m_light.m_direction;
	lightInfo.specularColor = m_light.m_specularColor;
	lightInfo.specularPower = m_light.m_specularPower;
	m_bufferManager->SetConstantBufferData("Light", &lightInfo, Graphics::ShaderType::Pixel);

	m_bufferManager->DrawIndexed(info.size, info.vertexOffset, info.indexOffset);
}

//void Graphics::DX11Graphics::ResizeWindow(const uint32_t width, const uint32_t height)
//{
//	HRESULT result;
//	//Reset all objects that use window size
//	m_renderTargetView->Release();
//	result = m_swapChain->ResizeBuffers(m_bufferCount, width, height, DXGI_FORMAT_UNKNOWN, NULL);
//	CheckSuccess(result);
//
//	CreateRTV();
//	m_depthStencilState->OnWindowResize(m_renderTargetView, width, height);
//	CreateViewport(width, height);
//	//Is optional
//	m_camera->SetResolution(width, height);
//}

void Graphics::DX11Graphics::Reset()
{
	m_bFilled = false;
	m_vertexManager->Reset();
	m_modelDrawCache_3D.clear();
	m_cachedModels_3D.clear();
	Model::MeshLoader::Clear();
	DX11TextureManager::Clear();
}

void Graphics::DX11Graphics::FillBuffers()
{
	std::vector<Model::Vertex> vertices;
	std::vector<Model::Index> indices;
	m_vertexManager->GetBufferData(vertices, indices);
	if (!indices.empty())
	{
		m_bufferManager->AddVertexData(vertices.data(), sizeof(Model::Vertex), (const unsigned int)vertices.size());
		m_bufferManager->AddIndexData(indices.data(), sizeof(Model::Index), (const unsigned int)indices.size());
		m_bufferManager->CompileVertexData();
		m_bFilled = true;
	}

}

void Graphics::DX11Graphics::CreateConstBuffers()
{
	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(MatrixBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	cbbd.StructureByteStride = 0;
	m_bufferManager->CreateConstantBuffer(cbbd, "Matrix");

	D3D11_BUFFER_DESC tbd = { 0 };
	tbd.Usage = D3D11_USAGE_DYNAMIC;
	tbd.ByteWidth = sizeof(PF_PixelBuffer);
	tbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tbd.MiscFlags = 0;
	tbd.StructureByteStride = 0;
	m_bufferManager->CreateConstantBuffer(tbd, "Light");

	D3D11_BUFFER_DESC txtbd = { 0 };
	txtbd.Usage = D3D11_USAGE_DYNAMIC;
	txtbd.ByteWidth = sizeof(Float8);
	txtbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	txtbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	txtbd.MiscFlags = 0;
	txtbd.StructureByteStride = 0;
	m_bufferManager->CreateConstantBuffer(txtbd, "float8");
}