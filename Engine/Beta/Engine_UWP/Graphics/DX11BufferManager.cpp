#include "DX11BufferManager.h"
#include "Model.h"


Graphics::DX11BufferManager::DX11BufferManager(ID3D11Device3* device, ID3D11DeviceContext3* context, std::shared_ptr<DX11ConstantBuffer>& constbuff)
{
	m_device = device;
	m_context = context;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_cbManager = constbuff;

	//DEFAULT Vertex Buffer Description
	m_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_vertexBufferDesc.ByteWidth = 0;
	m_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_vertexBufferDesc.CPUAccessFlags = 0;
	m_vertexBufferDesc.MiscFlags = 0;
	m_vertexBufferDesc.StructureByteStride = 0;

	//DEFAULT Index Buffer Description
	m_indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_indexBufferDesc.ByteWidth = 0;
	m_indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_indexBufferDesc.CPUAccessFlags = 0;
	m_indexBufferDesc.MiscFlags = 0;
	m_indexBufferDesc.StructureByteStride = 0;

	//DEFAULT Instance Buffer Description
	m_instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_instanceBufferDesc.ByteWidth = 0;
	m_instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_instanceBufferDesc.MiscFlags = 0;
	m_instanceBufferDesc.StructureByteStride = 0;
}

Graphics::DX11BufferManager::~DX11BufferManager()
{
	ClearVertexBuffers();
}

void Graphics::DX11BufferManager::AddVertexData(const void* data, const uint32_t vertexSize, const uint32_t vertexBufferSize)
{
	m_vertexData = { 0 };
	m_vertexData.pSysMem = data;
	m_vertexBufferDesc.ByteWidth = vertexSize * vertexBufferSize;
	m_vertexStride = vertexSize;
}

void Graphics::DX11BufferManager::AddIndexData(const void * data, const uint32_t indexSize, const uint32_t indexBufferSize)
{
	m_indexData = { 0 };
	m_indexData.pSysMem = data;
	m_indexBufferDesc.ByteWidth = indexSize * indexBufferSize;
}

void Graphics::DX11BufferManager::AddInstanceData(const void * data, const uint32_t instanceSize, const uint32_t instanceCount)
{
	//m_instanceData = { 0 };
	//m_instanceData.pSysMem = data;
	//m_instanceBufferDesc.ByteWidth = instanceSize * instanceCount;
	//m_instanceStride = instanceSize;
}

void Graphics::DX11BufferManager::UpdateInstanceData(const void * data, const uint32_t instanceCount, const uint32_t instanceSize)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//m_context->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//memcpy(mappedResource.pData, data, instanceCount * instanceSize);
	//m_context->Unmap(m_instanceBuffer, 0);
}

void Graphics::DX11BufferManager::SetVertexBufferDesc(const D3D11_BUFFER_DESC & desc)
{
	m_vertexData = { 0 };
	m_vertexBufferDesc = desc;
}

void Graphics::DX11BufferManager::SetIndexBufferDesc(const D3D11_BUFFER_DESC & desc)
{
	m_indexData = { 0 };
	m_indexBufferDesc = desc;
}

void Graphics::DX11BufferManager::SetDrawTopology(const D3D_PRIMITIVE_TOPOLOGY style)
{
	m_context->IASetPrimitiveTopology(style);
}

void Graphics::DX11BufferManager::SetAsActive()
{
	//Instancing
	ID3D11Buffer* bufferPointers[2] = { m_vertexBuffer , m_instanceBuffer };
	unsigned int strides[2] = { m_vertexStride, m_instanceStride };
	unsigned int offsets[2] = { 0,0 };
	UINT offset = 0;

	m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &offset);
	m_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::DX11BufferManager::DrawIndexedInstanced(uint32_t IndexCountPerInstance, uint32_t InstanceCount, uint32_t StartIndexLocation, uint32_t BaseVertexLocation, uint32_t StartInstanceLocation)
{
	m_cbManager->ResetCounter();
	m_context->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

void Graphics::DX11BufferManager::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	m_cbManager->ResetCounter();
	m_context->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void Graphics::DX11BufferManager::Draw(uint32_t size, uint32_t offset)
{
	m_cbManager->ResetCounter();
	m_context->Draw(size, offset);
}

bool Graphics::DX11BufferManager::CreateBuffers()
{
	//ClearVertexBuffers before intialize for safety
	ClearVertexBuffers();

	HRESULT result;
	//Create Vertex Buffer
	result = m_device->CreateBuffer(&m_vertexBufferDesc, &m_vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not create the Vertex Buffer", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	//Create Index Buffer
	result = m_device->CreateBuffer(&m_indexBufferDesc, &m_indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not create the Index Buffer", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	////Create Instance Buffer
	//result = m_device->CreateBuffer(&m_instanceBufferDesc, &m_instanceData, &m_instanceBuffer);
	//if (FAILED(result))
	//{
	//	MessageBox(NULL, L"Could not create the Instance Buffer", L"Error", MB_ICONERROR | MB_OK);
	//	return false;
	//}

	SetAsActive();
	return true;
}

void Graphics::DX11BufferManager::ClearVertexBuffers()
{
	CLOSE_COM_PTR(m_vertexBuffer);
	CLOSE_COM_PTR(m_indexBuffer);
}

void Graphics::DX11BufferManager::CreateConstantBuffer(const D3D11_BUFFER_DESC & desc, const std::string uniqueName)
{
	if (m_cbManager)
		m_cbManager->CreateBuffer(desc, uniqueName);
}

void Graphics::DX11BufferManager::SetConstantBufferData(const std::string & uniqueName, void * data, const ShaderType type)
{
	if (m_cbManager)
		m_cbManager->SetBufferData(uniqueName, data, type);
}