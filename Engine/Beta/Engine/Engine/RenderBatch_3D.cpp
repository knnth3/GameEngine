#include "RenderBatch_3D.h"
#include "MeshLoader.h"


Engine::RenderBatch_3D::RenderBatch_3D(ID3D11Device3* device, ID3D11DeviceContext3* context):
	m_shaderLib(device, context),
	m_bufferLib(device, context),
	m_rssLib(device, context),
	m_vertexManager(MAX_INSTANCES)
{
	m_device = device;
	m_context = context;
	m_bWireframe = false;
}

bool Engine::RenderBatch_3D::Initialize(std::shared_ptr<Camera>& camera)
{
	m_textureLib = std::make_shared<TextureLibrary>(m_device, m_context);

	m_camera = camera;
	if(!m_camera)
		return false;

	std::string mesh    = "Assets/Models/cube.sef";
	std::string texture = "Assets/textures/Default/white.png";
	std::string vs      = "Assets/Shaders/3D_VertexShader.cso";
	std::string ps      = "Assets/Shaders/3D_PixelShader.cso";
	std::string vsFallback = "Assets/Shaders/3D_VertexShader.hlsl";
	std::string psFallback = "Assets/Shaders/3D_PixelShader.hlsl";
	try
	{
		std::string error;
		//Shaders
		if (!m_shaderLib.Initialize(vs, ps, &error, vsFallback, psFallback))
			throw std::runtime_error("Shader Library could not initialze.\n" + error);

		if (!CreateShaders(error))
			throw std::runtime_error("Could not create nessesary shaders.\n" + error);

		//Textures
		if(!m_textureLib->Initialize(texture))
			throw std::runtime_error("Texture library could not initialize.");

		//Buffers
		if (!m_bufferLib.Initialize())
			throw std::runtime_error("Buffer Library could not initialze.");

		m_bufferLib.GetVertexBuffer()->SetStride(sizeof(Vertex));

		if(!CreateConstBuffers())
			throw std::runtime_error("Could not create constant buffers.");

		//Rasterization states
		if(!m_rssLib.Initialize())
			throw std::runtime_error("RSS library could not initialize.");

		if(!CreateRSSStates())
			throw std::runtime_error("RSS states not created");

		//Mesh loader
		if(!MeshLoader::Initialize(mesh))
			throw std::runtime_error("MeshLoader initialization failed.");

		return true;
	}
	catch (const std::runtime_error& e)
	{
		std::string message = e.what();
		OpenDialog(L"Initialization of Graphics failed.", To_wstr(message).c_str());
		return false;
	}
}

void Engine::RenderBatch_3D::Draw(const Model & model)
{
	m_vertexManager.AddModel(model);
}

void Engine::RenderBatch_3D::Wireframe(bool value)
{
	m_bWireframe = value;
}

void Engine::RenderBatch_3D::ToggleWireframe()
{
	m_bWireframe = !m_bWireframe;
}

void Engine::RenderBatch_3D::ProcessScene()
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
			LoadBatchInfo(batch);
			RenderBatch(batch.Info);
		}
	}
}

const std::shared_ptr<Engine::TextureLibrary>& Engine::RenderBatch_3D::GetTextureLibrary()
{
	return m_textureLib;
}

void Engine::RenderBatch_3D::LoadBatchInfo(Batch & batch)
{
	//Fill Constant buffer with instance data
	PBInfo batchInfo;
	batchInfo.flags[0] = false;
	batchInfo.view = glm::transpose(m_camera->GetViewMatrix());
	batchInfo.projection = glm::transpose(m_camera->Get3DProjectionMatrix());
	batchInfo.camera = glm::vec4(m_camera->GetFocusPoint(),1.0f);
	batchInfo.flags[0] = batch.Info.UsingVertexColors;
	if (batch.JointTransforms)
	{
		for (int index = 0; index < batch.JointTransforms->size(); index++)
		{
			batchInfo.jointTransform[index] = glm::transpose(batch.JointTransforms->at(index));
		}
	}

	for (size_t index = 0; index < batch.Data.size(); index++)
		batchInfo.instances[index] = batch.Data[index];

	auto buffer = m_bufferLib.GetBuffer("BatchInfo");
	if (!buffer)
		throw std::exception();

	buffer->UpdateBuffer(&batchInfo, sizeof(batchInfo));

	//Reset counter since this is the last update call
	m_bufferLib.ResetBufferRefCount();
}

void Engine::RenderBatch_3D::RenderBatch(BatchInfo& info)
{
	//Set the relevant raster state active(if not already set)
	if (m_bWireframe)
		m_rssLib.SetAsActive("WIREFRAME");
	else if(info.Style & DRAW_STYLE_CULL_FRONT)
		m_rssLib.SetAsActive("CULL_FRONT");
	else
		m_rssLib.SetAsActive("");

	//Set the relevant texture active(if not already set)
	if (info.Style & DRAW_STYLE_CUBEMAP)
		m_textureLib->SetAsActive("CUBEMAP");
	else
		m_textureLib->SetAsActive(info.Texture);

	//Set the relevant shader active(if not already set)
	if (info.Style & DRAW_STYLE_CUBEMAP)
		m_shaderLib.SetAsActive("CUBEMAP");
	else
		m_shaderLib.SetAsActive("");

	//Draw batch of instanced objects
	m_context->DrawIndexedInstanced(
		info.IndexCountPerInstance, 
		info.InstanceCount, 
		info.StartIndexLocation, 
		0, 0);
}

void Engine::RenderBatch_3D::Reset()
{
	m_textureLib->Clear();
	m_vertexManager.Reset();
}

bool Engine::RenderBatch_3D::FillBuffers()
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

bool Engine::RenderBatch_3D::CreateShaders(std::string& error)
{
	bool result = false;
	auto sbShader = m_shaderLib.CreateShader("CUBEMAP");
	if (sbShader)
	{
		result = sbShader->SetVertexShader("Assets/Shaders/Skybox_VertexShader.cso", error);
		if (!result)
		{
			result = sbShader->SetVertexShader("Assets/Shaders/Skybox_VertexShader.hlsl", error);
		}
		if (result)
		{
			result = sbShader->SetPixelShader("Assets/Shaders/Skybox_PixelShader.cso", error);

			if (!result)
			{
				result = sbShader->SetPixelShader("Assets/Shaders/Skybox_PixelShader.hlsl", error);
			}
		}
	}

	return result;
}

bool Engine::RenderBatch_3D::CreateConstBuffers()
{
	bool result = false;
	//Instance buffer
	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(PBInfo);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	cbbd.StructureByteStride = 0;
	auto buffer = m_bufferLib.CreateBuffer("BatchInfo", BUFFER_TYPE_CONSTANT, SHADER_TYPE_VERTEX);
	if (buffer)
	{
		result = buffer->CreateBuffer(NULL, sizeof(PBInfo));
	}
	return result;
}

bool Engine::RenderBatch_3D::CreateRSSStates()
{
	bool result = false;
	D3D11_RASTERIZER_DESC settings;

	//Wireframe
	settings = {};
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

	auto rss = m_rssLib.CreateRSS("WIREFRAME");
	result = rss->Initialize(settings);
	if (!result)
		return false;

	//FrontFace culling
	settings = {};
	settings.AntialiasedLineEnable = true;
	settings.CullMode = D3D11_CULL_FRONT;
	settings.DepthBias = 0;
	settings.DepthBiasClamp = 0.0f;
	settings.DepthClipEnable = true;
	settings.FillMode = D3D11_FILL_SOLID;
	settings.FrontCounterClockwise = true;
	settings.MultisampleEnable = true;
	settings.ScissorEnable = false;
	settings.SlopeScaledDepthBias = 0.0f;

	rss = m_rssLib.CreateRSS("CULL_FRONT");
	result = rss->Initialize(settings);
	if (!result)
		return false;

	m_rssLib.SetAsActive("Wireframe");
	return result;
}
