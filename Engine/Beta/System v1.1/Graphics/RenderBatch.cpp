#include "RenderBatch.h"
#include "MeshLoader.h"
#include "DX11TextureManager.h"

// --- Reversed iterable only C++ 14 & up
using std::rbegin;
using std::rend;

template <typename T>
struct reversion_wrapper { T& iterable; };

template <typename T>
auto begin(reversion_wrapper<T> w) { return rbegin(w.iterable); }

template <typename T>
auto end(reversion_wrapper<T> w) { return rend(w.iterable); }

template <typename T>
reversion_wrapper<T> reverse(T&& iterable) { return { iterable }; }


Graphics::DX11Graphics::DX11Graphics(std::shared_ptr<DX11BufferManager>& bufferManager, 
	std::shared_ptr<DX11ShaderManager> shaderManager,
	std::shared_ptr<DX11DepthStencilState>& depthStencilState,
	std::shared_ptr<DX11RasterStateManager>& rasterStateManager):
	m_bufferCount(3)
{
	m_bFilled = false;
	m_bDrawReversed = false;
	m_bufferManager = bufferManager;
	m_shaderManager = shaderManager;
	m_depthStencilState = depthStencilState;
	m_RSSManager = rasterStateManager;
	m_vertexManager = std::make_shared<VertexManager>();
}

Graphics::DX11Graphics::~DX11Graphics()
{
}

bool Graphics::DX11Graphics::Initialize(std::shared_ptr<Camera>& camera)
{
	m_camera = camera;
	if(!m_camera)
		return false;

	CreateShaders();
	CreateConstBuffers();
	return CreateRSSStates();
}

void Graphics::DX11Graphics::AddModel(std::shared_ptr<Model>& model)
{
	//Load up around 100 bools in a vector and if an exception happens in accesing, add another 50
	//If not, check number of active models on exception or every so submissions
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

void Graphics::DX11Graphics::DrawBackToFront(bool value)
{
	m_bDrawReversed = value;
}

void Graphics::DX11Graphics::Draw()
{
	//This code seems ok, maybe better ways to approach drawing
	//Maybe draw multiple instances at once?

	//Recompute if new data is available
	if (m_vertexManager->HasNewData())
		FillBuffers();

	//Draw objects
	if (m_bFilled)
	{

		//Process 3D objects
		if (m_bDrawReversed)
		{
			for (auto x : reverse(m_modelDrawCache_3D))
				ProcessObjects(m_cachedModels_3D[x]);
		}
		else
		{
			for (auto x : m_modelDrawCache_3D)
				ProcessObjects(m_cachedModels_3D[x]);
		}

		m_modelDrawCache_3D.clear();
	}
}

void Graphics::DX11Graphics::ProcessObjects(VertexManagerInfo & info)
{
	//Switch ok here but Draw style switch per item is super expensive
	//Optimization: Change to per batch style
	DrawStyle style = info.model->GetDrawStyle();
	m_bufferManager->SetAsActive(style);

	switch (style)
	{
	case Graphics::DrawStyle::POINT_3D:
		m_depthStencilState->SetDepthBufferStatus(true);
		ProcessObject_3DPointList(info);
		break;
	case Graphics::DrawStyle::POINT_2D:
		m_depthStencilState->SetDepthBufferStatus(false);
		ProcessObject_2DPointList(info);
		break;
	case Graphics::DrawStyle::LINE_3D:
		m_depthStencilState->SetDepthBufferStatus(true);
		ProcessObject_3DLineList(info);
		break;
	case Graphics::DrawStyle::LINE_2D:
		m_depthStencilState->SetDepthBufferStatus(false);
		ProcessObject_2DLineList(info);
		break;
	case Graphics::DrawStyle::TRIANGLE_3D:
		m_depthStencilState->SetDepthBufferStatus(true);
		ProcessObject_3DTriangles(info);
		break;
	case Graphics::DrawStyle::TRIANGLE_2D:
		m_depthStencilState->SetDepthBufferStatus(false);
		ProcessObject_2DTriangles(info);
		break;
	default:
		break;
	}
}

void Graphics::DX11Graphics::ProcessObject_3DTriangles(VertexManagerInfo& info)
{
	//Settings are the most expensive here!!
	//Need to think of a way to set once per overall use
	//Maybe group similar and then print (might not be ideal)

	//Grab model texture
	TextureID texture = info.model->GetTexture();

	//Activate texture
	SetActiveTexture(texture);

	//Set shader
	if (!m_shaderManager->SetActive("3D"))
		return;

	//Set RSS
	if (!SetRSStyleActive(info.model->GetRSS()))
		return;

	//Matrices should be set to Per-Batch not Per-Frame.
	//Optimization: Remove everything except for model matrix!
	//Light can be added later
	PerFrameBuffer matrices;
	matrices.world = glm::transpose(info.model->GetModelMatrix());

	if(info.model->IsUsingViewMatrix())
		matrices.view = glm::transpose(m_camera->GetViewMatrix());
	else
		matrices.view = glm::transpose(m_camera->GetIdentityMatrix());

	matrices.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
	matrices.color = info.model->GetColor();
	matrices.extraMemory = glm::vec4(m_camera->GetPosition(), 0.0f);
	m_bufferManager->SetConstantBufferData("Matrix", &matrices, Graphics::ShaderType::Vertex);

	m_bufferManager->DrawIndexed(info.size, info.vertexOffset, info.indexOffset);
}

void Graphics::DX11Graphics::ProcessObject_3DPointList(VertexManagerInfo & info)
{
	ProcessObject_3DTriangles(info);
}

void Graphics::DX11Graphics::ProcessObject_3DLineList(VertexManagerInfo & info)
{
	ProcessObject_3DTriangles(info);
}

void Graphics::DX11Graphics::ProcessObject_2DTriangles(VertexManagerInfo & info)
{
	//Same can be applied here as 3D

	//Grab model texture
	TextureID texture = info.model->GetTexture();

	//Activate texture
	SetActiveTexture(texture);

	//Set shader
	if (!m_shaderManager->SetActive("2D"))
		return;

	//Set RSS
	if (!SetRSStyleActive(info.model->GetRSS()))
		return;

	PerFrameBuffer matrices;
	matrices.world = glm::transpose(info.model->GetModelMatrix());
	matrices.view = glm::transpose(m_camera->GetIdentityMatrix());
	matrices.projection = m_camera->Get2DProjectionMatrix();
	matrices.color = info.model->GetColor();
	matrices.extraMemory = info.model->Get2DuvInfo();
	m_bufferManager->SetConstantBufferData("Matrix", &matrices, Graphics::ShaderType::Vertex);

	m_bufferManager->DrawIndexed(info.size, info.vertexOffset, info.indexOffset);
}

void Graphics::DX11Graphics::ProcessObject_2DPointList(VertexManagerInfo & info)
{
	ProcessObject_2DTriangles(info);
}

void Graphics::DX11Graphics::ProcessObject_2DLineList(VertexManagerInfo & info)
{
	ProcessObject_2DTriangles(info);
}

void Graphics::DX11Graphics::Reset()
{
	//Might not want to clear mesh loader and texture manager here.
	//Maybe more manual approach to delete (user controlled)

	m_bFilled = false;
	m_vertexManager->Reset();
	m_modelDrawCache_3D.clear();
	m_cachedModels_3D.clear();
	MeshLoader::Clear();
	DX11TextureManager::Clear();
}

void Graphics::DX11Graphics::FillBuffers()
{
	std::vector<Vertex> vertices;
	std::vector<Index> indices;
	m_vertexManager->GetBufferData(vertices, indices);
	if (!indices.empty())
	{
		m_bufferManager->AddVertexData(vertices.data(), sizeof(Vertex), (const unsigned int)vertices.size());
		m_bufferManager->AddIndexData(indices.data(), sizeof(Index), (const unsigned int)indices.size());
		m_bufferManager->CompileVertexData();
		m_bFilled = true;
	}

}

void Graphics::DX11Graphics::CreateShaders()
{
	std::wstring vsPath = L"EngineAssets/shaders/VertexShader.hlsl";
	std::wstring psPath = L"EngineAssets/shaders/PixelShader.hlsl";
	m_shaderManager->CreateShader("3D", vsPath, psPath);

	vsPath = L"EngineAssets/shaders/VertexShader2D.hlsl";
	psPath = L"EngineAssets/shaders/PixelShader2D.hlsl";
	m_shaderManager->CreateShader("2D", vsPath, psPath);
}

void Graphics::DX11Graphics::CreateConstBuffers()
{
	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(PerFrameBuffer);
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

bool Graphics::DX11Graphics::CreateRSSStates()
{
	bool result = false;
	// Setup the raster description which will determine how and what polygons will be drawn.
	RSSSettings rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_RSSManager->CreateRSS(rasterDesc, "2D");

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_RSSManager->CreateRSS(rasterDesc, "Wireframe");

	return result;
}

bool Graphics::DX11Graphics::SetRSStyleActive(RSS_STYLES style)
{
	bool success = true;
	switch (style)
	{
	case Graphics::RSS_DEFAULT:
		if (!m_RSSManager->SetActive("Default"))
			success = false;
		break;
	case Graphics::RSS_NO_CULL:
		if (!m_RSSManager->SetActive("2D"))
			success = false;
		break;
	case Graphics::RSS_WIREFRAME:
		if (!m_RSSManager->SetActive("Wireframe"))
			success = false;
		break;
	default:
		break;
	}
	if (style != RSS_DEFAULT && !success)
	{
		if (!m_RSSManager->SetActive("Default"))
			success = false;
	}

	return success;
}

void Graphics::DX11Graphics::SetActiveTexture(TextureID id)
{
	if(!DX11TextureManager::SetActive(id))
		DX11TextureManager::SetDefaultActive(DefaultTextures::MODEL);
}
