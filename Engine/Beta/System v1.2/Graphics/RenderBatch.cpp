#include "RenderBatch.h"
#include "MeshLoader.h"
#include "DX11TextureManager.h"

#define MAX_INSTANCES 500

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
	std::shared_ptr<DX11RasterStateManager>& rasterStateManager)
{
	m_bDrawReversed = false;
	m_bufferManager = bufferManager;
	m_shaderManager = shaderManager;
	m_depthStencilState = depthStencilState;
	m_RSSManager = rasterStateManager;
	m_vertexManager = std::make_shared<VertexManager>(MAX_INSTANCES);
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
	m_vertexManager->AddModel(model);
}

void Graphics::DX11Graphics::DrawBackToFront(bool value)
{
	m_bDrawReversed = value;
}

void Graphics::DX11Graphics::Draw()
{
	std::vector<PerInstanceInfo> instances;
	std::vector<BatchInfo> batchInfo;
	m_vertexManager->GetBatchData(instances, batchInfo);

	//Make sure there are instances to draw
	if (!instances.empty())
	{
		//Recompute new data
		if (FillBuffers(instances))
		{
			//Reset the current projection
			m_currentProj = MATRIX_NONE;

			//Draw each batch
			for (auto batch : batchInfo)
				ProcessObjects(batch);
		}
	}
}

void Graphics::DX11Graphics::ProcessObjects(BatchInfo & info)
{
	ProcessObject_3DTriangles(info);
}

void Graphics::DX11Graphics::ProcessObject_3DTriangles(BatchInfo& info)
{
	static bool in = false;
	//Set the relevant texture active(if not already set)
	SetActiveTexture(info.Texture);

	//Set the relevant shader active(if not already set)
	if (!m_shaderManager->SetActive("3D"))
		return;

	//Turn on the buffer if off
	m_depthStencilState->SetDepthBufferStatus(true);

	//Switch to 3D Projection Matrix
	if (m_currentProj != MATRIX_3D && !in)
	{
		PerFrameBuffer matrices;
		matrices.view = glm::transpose(m_camera->GetViewMatrix());
		matrices.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
		m_bufferManager->SetConstantBufferData("Matrix", &matrices, Graphics::ShaderType::Vertex);
		m_currentProj = MATRIX_3D;
		in = true;
	}

	//Draw batch of instanced objects
	m_bufferManager->DrawIndexedInstanced(
		info.IndexCountPerInstance, 
		info.InstanceCount, 
		info.StartIndexLocation, 
		info.BaseVertexLocation, 
		info.StartInstanceLocation);
}

void Graphics::DX11Graphics::ProcessObject_2DTriangles(BatchInfo & info)
{
	//Set the relevant texture active(if not already set)
	SetActiveTexture(info.Texture);

	//Set the relevant shader active(if not already set)
	if (!m_shaderManager->SetActive("2D"))
		return;

	//Turn off the buffer if on
	m_depthStencilState->SetDepthBufferStatus(true);

	//Switch to 2D Projection Matrix if not active
	if (m_currentProj != MATRIX_2D)
	{
		PerFrameBuffer matrices;
		matrices.view = glm::transpose(m_camera->GetViewMatrix());
		matrices.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
		m_bufferManager->SetConstantBufferData("Matrix", &matrices, Graphics::ShaderType::Vertex);
		m_currentProj = MATRIX_2D;
	}

	//Draw batch of instanced objects
	m_bufferManager->DrawIndexedInstanced(
		info.IndexCountPerInstance, 
		info.InstanceCount, 
		info.StartIndexLocation, 
		info.BaseVertexLocation, 
		info.StartInstanceLocation);
}

void Graphics::DX11Graphics::Reset()
{
	//Might not want to clear mesh loader and texture manager here.
	//Maybe more manual approach to delete (user controlled)
	//MeshLoader::Clear();
	//DX11TextureManager::Clear();

	m_vertexManager->Reset();
}

bool Graphics::DX11Graphics::FillBuffers(std::vector<PerInstanceInfo>& instances)
{
	bool m_bFilled = false;
	if (m_vertexManager->NewBatchInfo())
	{
		std::vector<Vertex> vertices;
		std::vector<Index> indices;
		m_vertexManager->GetVertexData(vertices, indices);
		m_bufferManager->AddVertexData(vertices.data(), sizeof(Vertex), (uint32_t)vertices.size());
		m_bufferManager->AddIndexData(indices.data(), sizeof(Index), (uint32_t)indices.size());
		m_bufferManager->AddInstanceData(instances.data(), sizeof(PerInstanceInfo), (uint32_t)instances.size());
		m_bFilled = m_bufferManager->CreateBuffers();
	}
	else
	{
		//Write Instance information to gfx
		m_bufferManager->UpdateInstanceData(instances.data(), (uint32_t)instances.size(), sizeof(PerInstanceInfo));
		m_bFilled = true;
	}
	return m_bFilled;
}

void Graphics::DX11Graphics::CreateShaders()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout =
	{
		//Vertex Info
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		//Instance Info
		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLDMAT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLDMAT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLDMAT", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLDMAT", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	std::wstring vsPath = L"EngineAssets/shaders/VertexShader.hlsl";
	std::wstring psPath = L"EngineAssets/shaders/PixelShader.hlsl";
	m_shaderManager->CreateShader("3D", vsPath, psPath, L"", newLayout);

	vsPath = L"EngineAssets/shaders/VertexShader2D.hlsl";
	psPath = L"EngineAssets/shaders/PixelShader2D.hlsl";
	m_shaderManager->CreateShader("2D", vsPath, psPath, L"", newLayout);
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
