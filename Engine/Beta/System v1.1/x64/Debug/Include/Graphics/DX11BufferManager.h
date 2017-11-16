#pragma once
#include <d3d11.h>
#include <map>
#include "Model.h"
#include "DllSettings.h"
#include "DX11ConstantBuffer.h"

namespace Graphics
{
	class DX11BufferManager
	{
	public:
		DX11BufferManager(ID3D11Device* device, ID3D11DeviceContext* context, std::shared_ptr<DX11ConstantBuffer>& constbuff);
		void AddVertexData(const void* data, const uint32_t vertexSize, const uint32_t vertexBufferSize);
		void AddIndexData(const void* data, const uint32_t indexSize, const uint32_t indexBufferSize);
		void SetVertexBufferDesc(const D3D11_BUFFER_DESC& desc);
		void SetIndexBufferDesc(const D3D11_BUFFER_DESC& desc);
		void CompileVertexData();
		void SetAsActive(const DrawStyle type);
		void ClearVertexBuffers();
		bool IsBufferEmpty();

		//Wrapper functions
		void CreateConstantBuffer(const D3D11_BUFFER_DESC& desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA* data = nullptr);
		void SetConstantBufferData(const std::string& uniqueName, void* data, const ShaderType type);

		//Preset Functions
		void DrawIndexed(uint32_t size, uint32_t vertexOffset, uint32_t indexOffset);
		void Draw(uint32_t size, uint32_t offset);

		~DX11BufferManager();

	private:
		bool m_isEmpty;
		uint32_t stride3d;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		D3D11_BUFFER_DESC m_vertexBufferDesc;
		D3D11_BUFFER_DESC m_indexBufferDesc;
		D3D11_SUBRESOURCE_DATA m_vertexBufferData;
		D3D11_SUBRESOURCE_DATA m_indexBufferData;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		std::shared_ptr<DX11ConstantBuffer> m_cbManager;
	};
}