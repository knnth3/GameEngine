#pragma once
#include "Library.h"
#include "DirectX_Buffer.h"


namespace Engine
{
	class BufferLibrary
	{
	public:
		BufferLibrary(ID3D11Device3 * device, ID3D11DeviceContext3 * context);
		bool Initialize();
		void DeleteBuffer(const std::string& uniqueName);
		const std::shared_ptr<DirectX_Buffer> GetBuffer(const std::string& uniqueName);
		const std::shared_ptr<DirectX_Buffer> CreateBuffer(const std::string& uniqueName, BufferType type, ShaderType usage = SHADER_TYPE_NONE);
		bool SetBufferActive(const std::string& uniqueName);
		void ClearBuffers();
		void ResetBufferRefCount();
		const std::shared_ptr<DirectX_Buffer>& GetVertexBuffer()const;
		const std::shared_ptr<DirectX_Buffer>& GetIndexBuffer()const;

	private:
		std::string m_activeBuffer;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		std::map<std::string, int> m_bufferCodex;
		std::shared_ptr<BufferRefCounter> m_bufferRefCounter;
		std::shared_ptr<DirectX_Buffer> m_vertexBuffer;
		std::shared_ptr<DirectX_Buffer> m_indexBuffer;
		Library<DirectX_Buffer> m_buffers;
	};

}
