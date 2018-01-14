#include "BufferLibrary.h"

using namespace Engine;

Engine::BufferLibrary::BufferLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context)
{
	m_device = device;
	m_context = context;
	m_activeBuffer = "";
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_bufferRefCounter = std::make_shared<BufferRefCounter>();
}

bool Engine::BufferLibrary::Initialize()
{
	m_vertexBuffer = std::make_shared<DirectX_Buffer>(m_device, m_context, BUFFER_TYPE_VERTEX, SHADER_TYPE_NONE);
	m_indexBuffer = std::make_shared<DirectX_Buffer>(m_device, m_context, BUFFER_TYPE_INDEX, SHADER_TYPE_NONE);
	if (!m_vertexBuffer && !m_indexBuffer)
		return false;

	return true;
}

const std::shared_ptr<DirectX_Buffer> Engine::BufferLibrary::GetBuffer(const std::string & uniqueName)
{
	int ref = 0;
	auto found = m_bufferCodex.find(uniqueName);
	if (found != m_bufferCodex.end())
	{
		auto entry = m_buffers.At(found->second);
		return entry;
	}
	return nullptr;
}

const std::shared_ptr<DirectX_Buffer> Engine::BufferLibrary::CreateBuffer(const std::string & uniqueName, BufferType type, ShaderType usage)
{
	int ref = 0;
	auto found = m_bufferCodex.find(uniqueName);
	if (found == m_bufferCodex.end())
	{
		ref = m_buffers.Emplace(m_device, m_context, type, usage);
		m_bufferCodex.emplace(uniqueName, ref);

		auto entry = m_buffers.At(ref);
		entry->AddRefCounter(m_bufferRefCounter);
		return entry;
	}
	return nullptr;
}

void Engine::BufferLibrary::DeleteBuffer(const std::string & uniqueName)
{
	auto found = m_bufferCodex.find(uniqueName);
	if (found != m_bufferCodex.end())
	{
		m_buffers.Remove(found->second);
	}
}

bool Engine::BufferLibrary::SetBufferActive(const std::string & uniqueName)
{
	if (m_activeBuffer.compare(uniqueName))
	{
		auto found = m_bufferCodex.find(uniqueName);
		if (found != m_bufferCodex.end())
		{
			m_activeBuffer = uniqueName;
			m_buffers.At(found->second)->SetAsActive();
			return true;
		}
		else if (!m_activeBuffer.empty())
		{
			auto defaultShader = m_buffers.At(0);
			if (!defaultShader)
				throw std::exception();

			m_activeBuffer = "";
			defaultShader->SetAsActive();
		}
		return false;
	}
	return true;
}

void Engine::BufferLibrary::ClearBuffers()
{
	for (auto& x : m_bufferCodex)
	{
		m_buffers.Remove(x.second);
	}
	m_bufferCodex.clear();
}

void Engine::BufferLibrary::ResetBufferRefCount()
{
	m_bufferRefCounter->ClearCount();
}

const std::shared_ptr<DirectX_Buffer>& Engine::BufferLibrary::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

const std::shared_ptr<DirectX_Buffer>& Engine::BufferLibrary::GetIndexBuffer() const
{
	return m_indexBuffer;
}
