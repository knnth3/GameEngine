#include "DX11Shader.h"


Lime::DX11Shader::DX11Shader(const LPCWSTR vsPath, const LPCWSTR psPath, ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_vsPath = vsPath;
	m_psPath = psPath;
	m_device = device;
	m_context = context;
	m_vertLayout = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;

	m_layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
}

HRESULT Lime::DX11Shader::Initialize()
{
	HRESULT result;
	ID3DBlob *vsBlob = nullptr;
	ID3DBlob *psBlob = nullptr;

	result = CompileShader(m_vsPath, "main", "vs_5_0", &vsBlob);
	CheckSuccess(result);

	result = CompileShader(m_psPath, "main", "ps_5_0", &psBlob);
	CheckSuccess(result);

	result = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vertexShader);
	CheckSuccess(result);

	result = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pixelShader);
	CheckSuccess(result);

	result = m_device->CreateInputLayout(m_layout.data(), (UINT)m_layout.size(), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &m_vertLayout);
	CheckSuccess(result);

	vsBlob->Release();
	psBlob->Release();

	return result;
}

void Lime::DX11Shader::Close()
{
	CLOSE_COM_PTR(m_vertexShader);
	CLOSE_COM_PTR(m_pixelShader);
	CLOSE_COM_PTR(m_vertLayout);
}

void Lime::DX11Shader::SetAsActive()
{
	m_context->IASetInputLayout(m_vertLayout);
	m_context->VSSetShader(m_vertexShader, 0, 0);
	m_context->PSSetShader(m_pixelShader, 0, 0);
}

Lime::DX11Shader::~DX11Shader()
{
	Close();
}

void Lime::DX11Shader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout)
{
	Close();
	m_layout = newLayout;
}

void Lime::DX11Shader::AttachConstBufferManager(std::shared_ptr<DX11ConstantBuffer>& constbuff)
{
	m_cbManager = constbuff;
}

void Lime::DX11Shader::CreateConstantBuffer(const D3D11_BUFFER_DESC & desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA * data)
{
	if (m_cbManager)
	{
		m_cbManager->CreateBuffer(desc, uniqueName, data);
	}
}

void Lime::DX11Shader::SetConstBufferData(const std::string & uniqueName, void * data, const ShaderType type)
{
	if (m_cbManager)
	{
		m_cbManager->SetBufferData(uniqueName, data, type);
	}
}

HRESULT Lime::DX11Shader::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint,
	LPCSTR profile, ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}
