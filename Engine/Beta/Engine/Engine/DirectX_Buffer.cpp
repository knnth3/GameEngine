#include "DirectX_Buffer.h"

using namespace Engine;

DirectX_Buffer::DirectX_Buffer(ID3D11Device3 * device, ID3D11DeviceContext3 * context, BufferType type, ShaderType usage)
{
	m_device = device;
	m_context = context;
	m_bAsConstantBuffer = true;
	m_buffer = nullptr;
	m_usage = usage;
	m_type = type;
	m_dataStride = 0;
	m_dataOffset = 0;
	m_desc = { 0 };
	m_subresource = { 0 };
	SetBufferDescription(type);
}

bool Engine::DirectX_Buffer::AddRefCounter(std::shared_ptr<BufferRefCounter> counter)
{
	if (!counter)
		return false;

	m_counter = counter;
	return true;
}

void Engine::DirectX_Buffer::SetAsActive()
{
	switch (m_type)
	{
	case BUFFER_TYPE_VERTEX:
		if (m_dataStride == 0)
			throw std::exception();

		m_context->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), &m_dataStride, &m_dataOffset);
		break;

	case BUFFER_TYPE_INDEX:
		m_context->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, m_dataOffset);
		break;
	}
}

bool Engine::DirectX_Buffer::CreateBuffer(const void* data, const uint32_t bufferSize)
{
	HRESULT result;
	if (m_buffer)
		m_buffer.Reset();

	m_desc.ByteWidth = bufferSize;
	if (data)
	{
		m_subresource.pSysMem = data;
		result = m_device->CreateBuffer(&m_desc, &m_subresource, &m_buffer);
	}
	else
		result = m_device->CreateBuffer(&m_desc, NULL, &m_buffer);

	CheckSuccess(result, L"Constant Buffer");

	return SUCCEEDED(result);
}

void Engine::DirectX_Buffer::UpdateBuffer(const void * data, const uint32_t bufferSize)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D11_MAP flags = D3D11_MAP_WRITE_DISCARD;
	result = m_context->Map(m_buffer.Get(), 0, flags, 0, &mappedResource);
	CheckSuccess(result, L"Map Constant Buffer");
	memcpy(mappedResource.pData, data, bufferSize);
	m_context->Unmap(m_buffer.Get(), 0);

	if (m_usage != SHADER_TYPE_NONE)
	{
		if (!m_counter)
			throw std::exception();

		int count = m_counter->CheckOutKey(m_usage);
		switch (m_usage)
		{
		case SHADER_TYPE_VERTEX:
			m_context->VSSetConstantBuffers(count, 1, m_buffer.GetAddressOf());
			break;
		case SHADER_TYPE_PIXEL:
			m_context->PSSetConstantBuffers(count, 1, m_buffer.GetAddressOf());
			break;
		case SHADER_TYPE_GEOMETRY:
			m_context->GSSetConstantBuffers(count, 1, m_buffer.GetAddressOf());
			break;
		default:
			break;
		}
	}
}

void Engine::DirectX_Buffer::SetStride(uint32_t stride)
{
	m_dataStride = stride;
}

void Engine::DirectX_Buffer::SetOffset(uint32_t offset)
{
	m_dataOffset = offset;
}

void Engine::DirectX_Buffer::SetBufferDescription(BufferType type)
{
	switch (type)
	{
	case BUFFER_TYPE_CONSTANT:
		m_desc.Usage = D3D11_USAGE_DYNAMIC;
		m_desc.ByteWidth = 0;
		m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_desc.MiscFlags = 0;
		m_desc.StructureByteStride = 0;
		break;

	case BUFFER_TYPE_VERTEX:
		m_desc.Usage = D3D11_USAGE_DEFAULT;
		m_desc.ByteWidth = 0;
		m_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_desc.CPUAccessFlags = 0;
		m_desc.MiscFlags = 0;
		m_desc.StructureByteStride = 0;
		break;

	case BUFFER_TYPE_INDEX:
		m_desc.Usage = D3D11_USAGE_DEFAULT;
		m_desc.ByteWidth = 0;
		m_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_desc.CPUAccessFlags = 0;
		m_desc.MiscFlags = 0;
		m_desc.StructureByteStride = 0;
		break;
	}
}

void Engine::BufferRefCounter::ClearCount()
{
	m_refCount.clear();
}

int Engine::BufferRefCounter::CheckOutKey(ShaderType type)
{
	auto found = m_refCount.find(type);
	if (found != m_refCount.end())
		return (int)found->second++;

	m_refCount.emplace(type, 0u);
	return 0;
}
