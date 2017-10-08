#include "DX11ConstantBuffer.h"


Lime::DX11ConstantBuffer::DX11ConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_device = device;
	m_context = context;
	ResetCounter();
}

void Lime::DX11ConstantBuffer::Close()
{
	for (auto x : m_constBuffers)
	{
		CLOSE_COM_PTR(x.second);
	}
	m_constBuffers.clear();
}

Lime::DX11ConstantBuffer::~DX11ConstantBuffer()
{
	Close();
}

void Lime::DX11ConstantBuffer::CreateBuffer(const D3D11_BUFFER_DESC & desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA * data)
{
	HRESULT result;
	ID3D11Buffer* constBuffer = nullptr;
	result = m_device->CreateBuffer(&desc, data, &constBuffer);
	CheckSuccess(result);

	m_constBuffers.insert(std::pair<std::string, ID3D11Buffer*>(uniqueName, constBuffer));
	m_constBufferSizes.insert(std::pair<std::string, uint32_t>(uniqueName, desc.ByteWidth));
}

void Lime::DX11ConstantBuffer::SetBufferData(const std::string & uniqueName, void * data, const ShaderType type)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ID3D11Buffer* constBuffer = m_constBuffers[uniqueName];
	uint32_t bufferSize = m_constBufferSizes[uniqueName];
	result = m_context->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CheckSuccess(result);
	memcpy(mappedResource.pData, data, bufferSize);
	m_context->Unmap(constBuffer, 0);

	switch (type)
	{
	case ShaderType::Vertex:
		m_context->VSSetConstantBuffers(m_vertexCounter, 1, &constBuffer);
		m_vertexCounter++;
		break;
	case ShaderType::Pixel:
		m_context->PSSetConstantBuffers(m_pixelCounter, 1, &constBuffer);
		m_pixelCounter++;
		break;
	default:
		break;
	}
}

void Lime::DX11ConstantBuffer::ResetCounter()
{
	m_vertexCounter = 0;
	m_pixelCounter = 0;
}
