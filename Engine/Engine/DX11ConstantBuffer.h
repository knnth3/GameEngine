#pragma once
#include <d3d11.h>
#include <map>
#include "Shared.h"


namespace Lime
{
	enum class ShaderType
	{
		Unknown = 0,
		Vertex,
		Pixel,
		Geometry
	};

	class DX11ConstantBuffer
	{
		friend class DX11BufferManager;
		friend class DX11Shader;
	public:
		DX11ConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* context);
		void Close();
		~DX11ConstantBuffer();

	protected:
		//Create
		void CreateBuffer(const D3D11_BUFFER_DESC& desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA* data = nullptr);

		//Modify
		void SetBufferData(const std::string& uniqueName, void* data, const ShaderType type);

		void ResetCounter();

	private:
		int m_vertexCounter;
		int m_pixelCounter;
		int m_geometryCounter;

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;

		std::map<std::string, ID3D11Buffer*> m_constBuffers;
		std::map<std::string, uint32_t> m_constBufferSizes;

	};
}

