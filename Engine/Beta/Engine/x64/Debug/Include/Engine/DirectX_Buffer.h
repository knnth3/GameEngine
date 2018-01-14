#pragma once
#include "WindowsAdditionals.h"

namespace Engine
{
	enum BufferType
	{
		BUFFER_TYPE_CONSTANT = 0,
		BUFFER_TYPE_VERTEX,
		BUFFER_TYPE_INDEX
	};
	enum ShaderType
	{
		SHADER_TYPE_NONE = 0,
		SHADER_TYPE_VERTEX,
		SHADER_TYPE_PIXEL,
		SHADER_TYPE_GEOMETRY
	};

	class BufferRefCounter
	{
	public:
		//Clear the counter every frame!
		void ClearCount();
		int CheckOutKey(ShaderType type);
	private:
		std::map<ShaderType, unsigned int> m_refCount;
	};

	class DirectX_Buffer
	{
	public:
		DirectX_Buffer(ID3D11Device3 * device, ID3D11DeviceContext3 * context, BufferType type, ShaderType usage);
		bool AddRefCounter(std::shared_ptr<BufferRefCounter> counter);
		void SetAsActive();
		bool CreateBuffer(const void* data, const uint32_t bufferSize);
		void UpdateBuffer(const void* data, const uint32_t bufferSize);
		void SetStride(uint32_t stride);
		void SetOffset(uint32_t offset);
	private:
		void SetBufferDescription(BufferType type);

		bool m_bAsConstantBuffer;
		uint32_t m_dataOffset;
		uint32_t m_dataStride;
		ShaderType m_usage;
		BufferType m_type;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		D3D11_BUFFER_DESC m_desc;
		D3D11_SUBRESOURCE_DATA m_subresource;
		std::shared_ptr<BufferRefCounter> m_counter;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	};
}

