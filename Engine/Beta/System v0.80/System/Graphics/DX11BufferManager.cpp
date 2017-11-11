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

Graphics::DX11BufferManager::DX11BufferManager(ID3D11Device* device, ID3D11DeviceContext* context, std::shared_ptr<DX11ConstantBuffer>& constbuff)
{
	m_device = device;
	m_context = context;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_cbManager = constbuff;
	m_isEmpty = true;

	//Default Vertex Buffer Description
	m_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_vertexBufferDesc.ByteWidth = 0;
	m_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_vertexBufferDesc.CPUAccessFlags = 0;
	m_vertexBufferDesc.MiscFlags = 0;

	//Default Pixel Buffer Description
	m_indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_indexBufferDesc.ByteWidth = 0;
	m_indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_indexBufferDesc.CPUAccessFlags = 0;
	m_indexBufferDesc.MiscFlags = 0;
}

void Graphics::DX11BufferManager::AddVertexData(const void* data, const uint32_t vertexSize, const uint32_t vertexBufferSize)
{
	if (vertexBufferSize)
	{
		m_vertexBufferData = { 0 };
		m_vertexBufferData.pSysMem = data;
		m_vertexBufferDesc.ByteWidth = vertexSize * vertexBufferSize;
		stride3d = vertexSize;
		m_isEmpty = false;
	}

}

void Graphics::DX11BufferManager::AddIndexData(const void * data, const uint32_t indexSize, const uint32_t indexBufferSize)
{
	m_indexBufferData = { 0 };
	m_indexBufferData.pSysMem = data;
	m_indexBufferDesc.ByteWidth = indexSize * indexBufferSize;
}

void Graphics::DX11BufferManager::SetVertexBufferDesc(const D3D11_BUFFER_DESC & desc)
{
	m_vertexBufferData = { 0 };
	m_vertexBufferDesc = desc;
}

void Graphics::DX11BufferManager::SetIndexBufferDesc(const D3D11_BUFFER_DESC & desc)
{
	m_indexBufferData = { 0 };
	m_indexBufferDesc = desc;
}

void Graphics::DX11BufferManager::CompileVertexData()
{
	//ClearVertexBuffers before intialize for safety
	ClearVertexBuffers();

	if (!m_isEmpty)
	{
		HRESULT result;
		result = m_device->CreateBuffer(&m_vertexBufferDesc, &m_vertexBufferData, &m_vertexBuffer);
		CheckSuccess(result);
		result = m_device->CreateBuffer(&m_indexBufferDesc, &m_indexBufferData, &m_indexBuffer);
		CheckSuccess(result);
	}
}

void Graphics::DX11BufferManager::SetAsActive(const BufferTypes type)
{
	uint32_t offset = 0;
	m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride3d, &offset);
	m_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	switch (type)
	{
	case BUFFER_2D:
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		break;
	case BUFFER_3D:
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case BUFFER_LINE:
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	default:
		break;
	}
}

void Graphics::DX11BufferManager::ClearVertexBuffers()
{
	CLOSE_COM_PTR(m_vertexBuffer);
	CLOSE_COM_PTR(m_indexBuffer);
}

bool Graphics::DX11BufferManager::IsBufferEmpty()
{
	return m_isEmpty;
}

void Graphics::DX11BufferManager::CreateBuffer(const D3D11_BUFFER_DESC & desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA * data)
{
	if (m_cbManager)
		m_cbManager->CreateBuffer(desc, uniqueName, data);
}

void Graphics::DX11BufferManager::SetBufferData(const std::string & uniqueName, void * data, const ShaderType type)
{
	if (m_cbManager)
		m_cbManager->SetBufferData(uniqueName, data, type);
}

void Graphics::DX11BufferManager::DrawIndexed(uint32_t size, uint32_t vertexOffset, uint32_t indexOffset)
{
	m_cbManager->ResetCounter();
	m_context->DrawIndexed(size, vertexOffset, indexOffset);
}

void Graphics::DX11BufferManager::Draw(uint32_t size, uint32_t offset)
{
	m_cbManager->ResetCounter();
	m_context->Draw(size, offset);
}

Graphics::DX11BufferManager::~DX11BufferManager()
{
	ClearVertexBuffers();
}
