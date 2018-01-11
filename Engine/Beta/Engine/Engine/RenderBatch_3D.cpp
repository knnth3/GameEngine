#include "RenderBatch_3D.h"
#include "MeshLoader.h"


Graphics::RenderBatch_3D::RenderBatch_3D(ID3D11Device3* device, ID3D11DeviceContext3* context):
	m_shaderLib(device, context),
	m_bufferLib(device, context),
	m_rssLib(device, context),
	m_vertexManager(MAX_INSTANCES)
{
	m_device = device;
	m_context = context;
	m_bWireframe = false;
}

bool Graphics::RenderBatch_3D::Initialize(std::shared_ptr<Camera>& camera)
{
	m_camera = camera;
	if(!m_camera)
		return false;

	std::string diffuse = "Assets/textures/Default/diffuse.png";
	std::string normal = "Assets/textures/Default/normal.png";
	std::string emissive = "Assets/textures/Default/emissive.png";
	std::string roughness = "Assets/textures/Default/roughness.png";
	std::string metallic = "Assets/textures/Default/metallic.png";
	m_textureLib = std::make_shared<TextureLibrary>(m_device, m_context);
	if (!m_textureLib->Initialize(diffuse, normal, emissive, roughness, metallic))
		return false;

	if (!m_shaderLib.Initialize("Assets/Shaders/VertexShader_3D.hlsl", "Assets/Shaders/PixelShader_3D.hlsl"))
		return false;

	if (!m_bufferLib.Initialize())
		return false;

	if (!m_rssLib.Initialize())
		return false;

	m_bufferLib.GetVertexBuffer()->SetStride(sizeof(Vertex));

	CreateShaders();
	CreateConstBuffers();
	CreateRSSStates();
	return true;
}

void Graphics::RenderBatch_3D::Draw(const Model& model)
{
	m_vertexManager.AddModel(model);
}

void Graphics::RenderBatch_3D::Wireframe(bool value)
{
	m_bWireframe = value;
}

void Graphics::RenderBatch_3D::ProcessScene()
{
	//Get info
	std::vector<Batch> batchLib;
	m_vertexManager.GetBatchData(batchLib);

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

const std::shared_ptr<Graphics::TextureLibrary>& Graphics::RenderBatch_3D::GetTextureLibrary()
{
	return m_textureLib;
}

void Graphics::RenderBatch_3D::ProcessObjects(Batch & batch)
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

		auto buffer = m_bufferLib.GetBuffer("BatchInfo");
		if (!buffer)
			throw std::exception();

		buffer->UpdateBuffer(&batchInfo, sizeof(batchInfo));
		m_bufferLib.ResetBufferRefCount();
		ProcessObject_3DTriangles(batch.info);
	}
}

void Graphics::RenderBatch_3D::ProcessObject_3DTriangles(BatchInfo& info)
{
	std::string rss = "";
	if (m_bWireframe)
		rss = "Wireframe";
	//Set the relevant texture active(if not already set)
	if (!m_textureLib->SetAsActive(info.Texture))
	{
		//Do something
	}

	if (!m_rssLib.SetAsActive(rss))
	{
		//Do something
	}

	//Set the relevant shader active(if not already set)
	if (info.UsingVertexColors)
		m_shaderLib.SetAsActive("VertexColor");
	else
		m_shaderLib.SetAsActive(""); 

	//Draw batch of instanced objects
	m_context->DrawIndexedInstanced(
		info.IndexCountPerInstance, 
		info.InstanceCount, 
		info.StartIndexLocation, 
		0, 0);
}

void Graphics::RenderBatch_3D::Reset()
{
	m_textureLib->Clear();
	m_vertexManager.Reset();
}

bool Graphics::RenderBatch_3D::FillBuffers()
{
	bool m_bFilled = true;
	if (m_vertexManager.NewBatchInfo())
	{
		std::vector<Vertex> vertices;
		std::vector<Index> indices;
		m_vertexManager.GetVertexData(vertices, indices);
		auto vbuffer = m_bufferLib.GetVertexBuffer();
		auto ibuffer = m_bufferLib.GetIndexBuffer();

		bool vfilled = vbuffer->CreateBuffer(vertices.data(), (uint32_t)vertices.size() * sizeof(Vertex));
		bool ifilled = ibuffer->CreateBuffer(indices.data(), (uint32_t)indices.size() * sizeof(Index));
		m_bFilled = vfilled && ifilled;
		if (m_bFilled)
		{
			vbuffer->SetAsActive();
			ibuffer->SetAsActive();
		}

	}
	return m_bFilled;
}

void Graphics::RenderBatch_3D::CreateShaders()
{
	//std::wstring path;

	//path = L"Assets/shaders/3D Shader.hlsl";
	//m_shaderManager->CreateShader("3D", path);

	//path = L"Assets/shaders/vertex_color_shader.hlsl";
	//m_shaderManager->CreateShader("VertexColor", path);
}

void Graphics::RenderBatch_3D::CreateConstBuffers()
{
	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(PBInfo);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	cbbd.StructureByteStride = 0;
	auto buffer = m_bufferLib.CreateBuffer("BatchInfo", BUFFER_TYPE_CONSTANT, SHADER_TYPE_VERTEX);
	if (!buffer->CreateBuffer(NULL, sizeof(PBInfo)))
		throw std::exception();
}

bool Graphics::RenderBatch_3D::CreateRSSStates()
{
	bool result = false;

	//Wireframe
	D3D11_RASTERIZER_DESC settings;
	settings.AntialiasedLineEnable = true;
	settings.CullMode = D3D11_CULL_NONE;
	settings.DepthBias = 0;
	settings.DepthBiasClamp = 0.0f;
	settings.DepthClipEnable = true;
	settings.FillMode = D3D11_FILL_WIREFRAME;
	settings.FrontCounterClockwise = true;
	settings.MultisampleEnable = true;
	settings.ScissorEnable = false;
	settings.SlopeScaledDepthBias = 0.0f;

	auto rss = m_rssLib.CreateRSS("Wireframe");
	result = rss->Initialize(settings);
	if (!result)
		return false;

	m_rssLib.SetAsActive("Wireframe");
	return result;
}
