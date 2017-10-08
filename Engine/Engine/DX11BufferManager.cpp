#include "DX11BufferManager.h"
#include "Model.h"

struct MatrixBuffer
{
	glm::mat4 world;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPos;
	float padding;
};

Lime::DX11BufferManager::DX11BufferManager(ID3D11Device* device, ID3D11DeviceContext* context, std::shared_ptr<DX11ConstantBuffer>& constbuff)
{
	m_device = device;
	m_context = context;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_cbManager = constbuff;
	//Default Vertex BMBuffer Description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = 0;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	//Default Pixel BMBuffer Description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = 0;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
}

void Lime::DX11BufferManager::AddVertexData(const void* data, const uint32_t vertexSize, const uint32_t vertexBufferSize)
{
	vertexBufferData = { 0 };
	vertexBufferData.pSysMem = data;
	vertexBufferDesc.ByteWidth = vertexSize * vertexBufferSize;
	stride = vertexSize;
}

void Lime::DX11BufferManager::AddIndexData(const void * data, const uint32_t indexSize, const uint32_t indexBufferSize)
{
	indexBufferData = { 0 };
	indexBufferData.pSysMem = data;
	indexBufferDesc.ByteWidth = indexSize * indexBufferSize;
}

void Lime::DX11BufferManager::SetVertexBufferDesc(const D3D11_BUFFER_DESC & desc)
{
	vertexBufferData = { 0 };
	vertexBufferDesc = desc;
}

void Lime::DX11BufferManager::SetIndexBufferDesc(const D3D11_BUFFER_DESC & desc)
{
	indexBufferData = { 0 };
	indexBufferDesc = desc;
}

void Lime::DX11BufferManager::CompileVertexData()
{
	//ClearVertexBuffers before intialize for safety
	ClearVertexBuffers();

	HRESULT result;
	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	CheckSuccess(result);
	result = m_device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);
	CheckSuccess(result);

	SetAsActive();
}

void Lime::DX11BufferManager::SetAsActive()
{
	UINT offset = 0;
	m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Lime::DX11BufferManager::ClearVertexBuffers()
{
	CLOSE_COM_PTR(m_vertexBuffer);
	CLOSE_COM_PTR(m_indexBuffer);
}

void Lime::DX11BufferManager::CreateBuffer(const D3D11_BUFFER_DESC & desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA * data)
{
	if (m_cbManager)
		m_cbManager->CreateBuffer(desc, uniqueName, data);
}

void Lime::DX11BufferManager::SetBufferData(const std::string & uniqueName, void * data, const ShaderType type)
{
	if (m_cbManager)
		m_cbManager->SetBufferData(uniqueName, data, type);
}

void Lime::DX11BufferManager::DrawIndexed(uint32_t size, uint32_t vertexOffset, uint32_t indexOffset)
{
	m_cbManager->ResetCounter();
	m_context->DrawIndexed(size, vertexOffset, indexOffset);
}

Lime::DX11BufferManager::~DX11BufferManager()
{
	ClearVertexBuffers();
}
