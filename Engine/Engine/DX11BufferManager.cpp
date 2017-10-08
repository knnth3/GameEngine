#include "DX11BufferManager.h"
#include "Model.h"

#define Check(x, lpctstr) \
	if(!(x)) { MessageBox(0, lpctstr, L"Error", MB_OK);}

#define CheckSuccess(hresult) \
	{_com_error err(hresult); Check(SUCCEEDED(hresult), err.ErrorMessage());}

#define CLOSE_COM_PTR(ptr) \
	if(ptr) { ptr->Release(); ptr = nullptr;}

Lime::DX11BufferManager::DX11BufferManager(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_device = device;
	m_context = context;
	m_vertexCounter = 0;
	m_pixelCounter = 0;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	//Default Vertex Buffer Description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = 0;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	//Default Pixel Buffer Description
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
	vertexBufferDesc.ByteWidth = sizeof(Model::Vertex) * 3;
	stride = vertexSize;
}

void Lime::DX11BufferManager::AddIndexData(const void * data, const uint32_t indexSize, const uint32_t indexBufferSize)
{
	indexBufferData = { 0 };
	indexBufferData.pSysMem = data;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 3;
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

void Lime::DX11BufferManager::CreateConstantBuffer(const D3D11_BUFFER_DESC& desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA* data)
{
	HRESULT result;
	ID3D11Buffer* constBuffer = nullptr;
	result = m_device->CreateBuffer(&desc, data, &constBuffer);
	CheckSuccess(result);

	m_constBuffers.insert(std::pair<std::string, ID3D11Buffer*>(uniqueName, constBuffer));
}

void Lime::DX11BufferManager::SetConstBufferData(const std::string& uniqueName, void * data, const Buffer type)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ID3D11Buffer* constBuffer = m_constBuffers[uniqueName];
	result = m_context->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CheckSuccess(result);
	mappedResource.pData = data;
	m_context->Unmap(constBuffer, 0);

	switch (type)
	{
	case Buffer::Vertex:
		m_context->VSSetConstantBuffers(0, 1, &constBuffer);
		m_vertexCounter++;
		break;
	case Buffer::Pixel:
		m_context->PSSetConstantBuffers(0, 1, &constBuffer);
		m_pixelCounter++;
		break;
	default:
		break;
	}
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

void Lime::DX11BufferManager::DrawIndexed(uint32_t size, uint32_t vertexOffset, uint32_t indexOffset)
{
	m_vertexCounter = 0;
	m_pixelCounter = 0;
	m_context->DrawIndexed(size, vertexOffset, indexOffset);
}

Lime::DX11BufferManager::~DX11BufferManager()
{
	ClearVertexBuffers();
	for (auto x : m_constBuffers)
	{
		CLOSE_COM_PTR(x.second);
	}
	m_constBuffers.clear();
}
