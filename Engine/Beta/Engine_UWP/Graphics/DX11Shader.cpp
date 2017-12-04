#include "DX11Shader.h"


Graphics::DX11Shader::DX11Shader(const LPCWSTR path, ID3D11Device3* device, ID3D11DeviceContext3* context, const bool bHasGS)
{
	m_filePath = path;
	m_hasGS = bHasGS;
	m_device = device;
	m_context = context;
	m_vertLayout = nullptr;
	m_vertexShader = nullptr;
	m_geometryShader = nullptr;
	m_pixelShader = nullptr;

	m_layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT    , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT  , 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
}

HRESULT Graphics::DX11Shader::Initialize()
{
	Close();
	HRESULT result;
	ID3DBlob *vsBlob = nullptr;
	ID3DBlob *gsBlob = nullptr;
	ID3DBlob *psBlob = nullptr;

	if (m_hasGS)
	{
		result = CompileShader(m_filePath, "gsMain", "gs_5_0", &gsBlob);
		CheckSuccess(result, m_filePath);

		result = m_device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), NULL, &m_geometryShader);
		CheckSuccess(result, m_filePath);

		gsBlob->Release();
	}

	result = CompileShader(m_filePath, "vsMain", "vs_5_0", &vsBlob);
	CheckSuccess(result, m_filePath);

	result = CompileShader(m_filePath, "psMain", "ps_5_0", &psBlob);
	CheckSuccess(result, m_filePath);

	result = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vertexShader);
	CheckSuccess(result, m_filePath);

	result = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pixelShader);
	CheckSuccess(result, m_filePath);

	result = m_device->CreateInputLayout(m_layout.data(), (uint32_t)m_layout.size(), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &m_vertLayout);
	CheckSuccess(result, m_filePath);

	vsBlob->Release();
	psBlob->Release();

	return result;
}

void Graphics::DX11Shader::Close()
{
	CLOSE_COM_PTR(m_vertexShader);
	CLOSE_COM_PTR(m_geometryShader);
	CLOSE_COM_PTR(m_pixelShader);
	CLOSE_COM_PTR(m_vertLayout);
}

void Graphics::DX11Shader::SetAsActive()
{
	m_context->IASetInputLayout(m_vertLayout);
	m_context->VSSetShader(m_vertexShader, 0, 0);
	m_context->PSSetShader(m_pixelShader, 0, 0);
	if (m_hasGS)
		m_context->GSSetShader(m_geometryShader, 0, 0);
	else
		m_context->GSSetShader(NULL, 0, 0);
}

Graphics::DX11Shader::~DX11Shader()
{
	Close();
}

void Graphics::DX11Shader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& newLayout)
{
	m_layout = newLayout;
}

HRESULT Graphics::DX11Shader::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint,
	LPCSTR profile, ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;
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
