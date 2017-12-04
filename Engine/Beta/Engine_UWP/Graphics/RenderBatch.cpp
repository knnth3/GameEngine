#include "RenderBatch.h"
#include "MeshLoader.h"
#include "TextureLoader.h"

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


Graphics::RenderBatch::RenderBatch(std::shared_ptr<DX11BufferManager>& bufferManager, std::shared_ptr<DX11ShaderManager>& shaderManager,
	std::shared_ptr<DX11RasterStateManager>& rssManager)
{
	m_bDrawReversed = false;
	m_bufferManager = bufferManager;
	m_shaderManager = shaderManager;
	m_RSSManager = rssManager;
	m_vertexManager = std::make_shared<VertexManager>(MAX_INSTANCES);
}

bool Graphics::RenderBatch::Initialize(std::shared_ptr<Camera>& camera)
{
	m_camera = camera;
	if(!m_camera)
		return false;

	CreateShaders();
	CreateConstBuffers();
	CreateRSSStates();
	return true;
}

void Graphics::RenderBatch::AddModel(Model& model)
{
	m_vertexManager->AddModel(model);
}

void Graphics::RenderBatch::DrawBackToFront(bool value)
{
	m_bDrawReversed = value;
}

void Graphics::RenderBatch::Draw()
{
	//Get info
	std::vector<Batch> batchLib;
	m_vertexManager->GetBatchData(batchLib);

	//Make sure there are instances to draw
	if (!batchLib.empty() && FillBuffers())
	{
		//Draw each batch
		for (auto batch : batchLib)
		{
			ProcessObjects(batch);
		}
	}
}

void Graphics::RenderBatch::ProcessObjects(Batch & batch)
{
	//Fill Constant buffer with instance data
	PBInfo batchInfo;
	batchInfo.flags[0] = false;
	batchInfo.view = glm::transpose(m_camera->GetViewMatrix());


	if (batch.info.Style == TRIANGLE_3D)
	{
		batchInfo.projection = glm::transpose(m_camera->Get3DProjectionMatrix());

		for (size_t index = 0; index < batch.data.size(); index++)
			batchInfo.instances[index] = batch.data[index];
		m_bufferManager->SetConstantBufferData("BatchInfo", &batchInfo, Graphics::ShaderType::Vertex);

		ProcessObject_3DTriangles(batch.info);
	}
	else if (batch.info.Style == TRIANGLE_2D)
	{
		batchInfo.projection = m_camera->Get2DProjectionMatrix();

		for (size_t index = 0; index < batch.data.size(); index++)
			batchInfo.instances[index] = batch.data[index];
		m_bufferManager->SetConstantBufferData("BatchInfo", &batchInfo, Graphics::ShaderType::Vertex);

		ProcessObject_2DTriangles(batch.info);
	}
}

void Graphics::RenderBatch::ProcessObject_3DTriangles(BatchInfo& info)
{
	//Set the relevant texture active(if not already set)
	SetActiveTexture(info.Texture);

	//Set the relevant shader active(if not already set)
	if (!m_shaderManager->SetActive("3D"))
		return;

	//Draw batch of instanced objects
	m_bufferManager->DrawIndexedInstanced(
		info.IndexCountPerInstance, 
		info.InstanceCount, 
		info.StartIndexLocation, 
		info.BaseVertexLocation, 
		0);
}

void Graphics::RenderBatch::ProcessObject_2DTriangles(BatchInfo & info)
{
	//Set the relevant texture active(if not already set)
	SetActiveTexture(info.Texture);

	//Set the relevant shader active(if not already set)
	if (!m_shaderManager->SetActive("2D"))
		return;

	//Draw batch of instanced objects
	m_bufferManager->DrawIndexedInstanced(
		info.IndexCountPerInstance,
		info.InstanceCount,
		info.StartIndexLocation,
		info.BaseVertexLocation,
		0);
}

void Graphics::RenderBatch::Reset()
{
	//Might not want to clear mesh loader and texture manager here.
	//Maybe more manual approach to delete (user controlled)
	//MeshLoader::Clear();
	//TextureLoader::Clear();

	m_vertexManager->Reset();
}

bool Graphics::RenderBatch::FillBuffers()
{
	bool m_bFilled = true;
	if (m_vertexManager->NewBatchInfo())
	{
		std::vector<Vertex> vertices;
		std::vector<Index> indices;
		m_vertexManager->GetVertexData(vertices, indices);
		m_bufferManager->AddVertexData(vertices.data(), sizeof(Vertex), (uint32_t)vertices.size());
		m_bufferManager->AddIndexData(indices.data(), sizeof(Index), (uint32_t)indices.size());
		m_bFilled = m_bufferManager->CreateBuffers();
	}
	return m_bFilled;
}

void Graphics::RenderBatch::CreateShaders()
{
	std::wstring path;

	path = L"Assets/shaders/3D Shader.hlsl";
	m_shaderManager->CreateShader("3D", path);

	path = L"Assets/shaders/2D Shader.hlsl";
	m_shaderManager->CreateShader("2D", path);
}

void Graphics::RenderBatch::CreateConstBuffers()
{
	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(PBInfo);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	cbbd.StructureByteStride = 0;
	m_bufferManager->CreateConstantBuffer(cbbd, "BatchInfo");
}

void Graphics::RenderBatch::SetActiveTexture(TextureID id)
{
	if(!TextureLoader::SetActive(id))
		TextureLoader::SetDefaultActive();
}

bool Graphics::RenderBatch::CreateRSSStates()
{
	bool result = false;
	RSSSettings settings;

	//3D default
	settings.AntialiasedLineEnable = false;
	settings.CullMode = D3D11_CULL_BACK;
	settings.DepthBias = 0;
	settings.DepthBiasClamp = 0.0f;
	settings.DepthClipEnable = true;
	settings.FillMode = D3D11_FILL_SOLID;
	settings.FrontCounterClockwise = true;
	settings.MultisampleEnable = false;
	settings.ScissorEnable = false;
	settings.SlopeScaledDepthBias = 0.0f;

	result = m_RSSManager->CreateRSS(settings, "3D");
	result = m_RSSManager->SetActive("3D");
	return result;
}
