#include "DX11Shader.h"


Graphics::DX11Shader::DX11Shader(const LPCWSTR vsPath, const LPCWSTR psPath, ID3D11Device* device, ID3D11DeviceContext* context, const LPCWSTR gsPath)
{
	m_vsPath = vsPath;
	m_psPath = psPath;
	m_gsPath = gsPath;
	m_device = device;
	m_context = context;
	m_vertLayout = nullptr;
	m_vertexShader = nullptr;
	m_geometryShader = nullptr;
	m_pixelShader = nullptr;
	if (m_gsPath[0] != '\0')
		m_hasGS = true;
	else
		m_hasGS = false;

	m_layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
}

HRESULT Graphics::DX11Shader::Initialize()
{
	HRESULT result;
	ID3DBlob *vsBlob = nullptr;
	ID3DBlob *gsBlob = nullptr;
	ID3DBlob *psBlob = nullptr;

	if(m_hasGS)
	{
		result = CompileShader(m_gsPath, "main", "gs_5_0", &gsBlob);
		CheckSuccess(result);

		result = m_device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), NULL, &m_geometryShader);
		CheckSuccess(result);

		gsBlob->Release();
	}

	result = CompileShader(m_vsPath, "main", "vs_5_0", &vsBlob);
	CheckSuccess(result);

	result = CompileShader(m_psPath, "main", "ps_5_0", &psBlob);
	CheckSuccess(result);

	result = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vertexShader);
	CheckSuccess(result);

	result = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pixelShader);
	CheckSuccess(result);

	result = m_device->CreateInputLayout(m_layout.data(), (uint32_t)m_layout.size(), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &m_vertLayout);
	CheckSuccess(result);

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

void Graphics::DX11Shader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> newLayout)
{
	Close();
	m_layout = newLayout;
}

void Graphics::DX11Shader::AttachConstBufferManager(std::shared_ptr<DX11ConstantBuffer>& constbuff)
{
	m_cbManager = constbuff;
}

void Graphics::DX11Shader::CreateConstantBuffer(const D3D11_BUFFER_DESC & desc, const std::string uniqueName, D3D11_SUBRESOURCE_DATA * data)
{
	if (m_cbManager)
	{
		m_cbManager->CreateBuffer(desc, uniqueName, data);
	}
}

void Graphics::DX11Shader::SetConstBufferData(const std::string & uniqueName, void * data, const ShaderType type)
{
	if (m_cbManager)
	{
		m_cbManager->SetBufferData(uniqueName, data, type);
	}
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
