#pragma once
#include <d3d11.h>
#include <map>
#include "Lime.h"

namespace Lime
{
	enum class Buffer
	{
		Unknown = 0,
		Vertex = 1,
		Pixel = 2
	};

	class DX11BufferManager
	{
	public:
		DX11BufferManager(ID3D11Device* device, ID3D11DeviceContext* context);
		void AddVertexData(const void* data, const uint32_t vertexSize, const uint32_t vertexBufferSize);
		void AddIndexData(const void* data, const uint32_t indexSize, const uint32_t indexBufferSize);
		void SetVertexBufferDesc(const D3D11_BUFFER_DESC& desc);
		void SetIndexBufferDesc(const D3D11_BUFFER_DESC& desc);
		void CreateConstantBuffer(const D3D11_BUFFER_DESC& desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA* data = nullptr);
		void SetConstBufferData(const std::string& uniqueName, void * data, const Buffer type);
		void CompileVertexData();
		void SetAsActive();
		void ClearVertexBuffers();

		//Preset Functions
		void DrawIndexed(uint32_t size, uint32_t vertexOffset, uint32_t indexOffset);

		~DX11BufferManager();

	private:

		int m_vertexCounter;
		int m_pixelCounter;
		UINT stride;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		D3D11_BUFFER_DESC vertexBufferDesc;
		D3D11_BUFFER_DESC indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexBufferData;
		D3D11_SUBRESOURCE_DATA indexBufferData;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		std::map<std::string, ID3D11Buffer*> m_constBuffers;
	};
}