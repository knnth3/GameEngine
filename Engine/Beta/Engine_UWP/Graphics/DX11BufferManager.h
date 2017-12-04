#pragma once
#include <D3D11_3.h>
#include <map>
#include "DX11ConstantBuffer.h"

namespace Graphics
{
	class DX11BufferManager
	{
	public:
		DX11BufferManager(ID3D11Device3* device, ID3D11DeviceContext3* context, std::shared_ptr<DX11ConstantBuffer>& constbuff);
		~DX11BufferManager();

		//Add Funcions
		void AddVertexData(const void* data, const uint32_t vertexSize, const uint32_t vertexBufferSize);
		void AddIndexData(const void* data, const uint32_t indexSize, const uint32_t indexBufferSize);
		void AddInstanceData(const void* data, const uint32_t instanceSize, const uint32_t instanceCount);
		void UpdateInstanceData(const void* data, const uint32_t instanceCount, const uint32_t instanceSize);

		//Set Settings
		void SetVertexBufferDesc(const D3D11_BUFFER_DESC& desc);
		void SetIndexBufferDesc(const D3D11_BUFFER_DESC& desc);
		void SetDrawTopology(const D3D_PRIMITIVE_TOPOLOGY style);
		void SetAsActive();

		//Preset Functions
		void DrawIndexedInstanced(
			uint32_t IndexCountPerInstance,
			uint32_t InstanceCount,
			uint32_t StartIndexLocation,
			uint32_t BaseVertexLocation,
			uint32_t StartInstanceLocation);
		void DrawIndexed(
			UINT IndexCount,
			UINT StartIndexLocation,
			INT BaseVertexLocation);
		void Draw(uint32_t size, uint32_t offset);

		//Usage Functions
		bool CreateBuffers();
		void ClearVertexBuffers();

		//Wrapper functions
		void CreateConstantBuffer(const D3D11_BUFFER_DESC& desc, const std::string uniqueName);
		void SetConstantBufferData(const std::string& uniqueName, void* data, const ShaderType type);


	private:

		uint32_t m_vertexStride;
		uint32_t m_instanceStride;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		D3D11_BUFFER_DESC m_vertexBufferDesc;
		D3D11_BUFFER_DESC m_indexBufferDesc;
		D3D11_BUFFER_DESC m_instanceBufferDesc;
		D3D11_SUBRESOURCE_DATA m_vertexData;
		D3D11_SUBRESOURCE_DATA m_indexData;
		D3D11_SUBRESOURCE_DATA m_instanceData;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		ID3D11Buffer* m_instanceBuffer;
		std::shared_ptr<DX11ConstantBuffer> m_cbManager;
	};
}