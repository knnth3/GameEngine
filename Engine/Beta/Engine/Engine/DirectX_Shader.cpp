#include "DirectX_Shader.h"
#include "Vertex.h"

using namespace Microsoft::WRL;

Engine::DirectX_Shader::DirectX_Shader(ID3D11Device3 * device, ID3D11DeviceContext3 * context)
{
	m_device = device;
	m_context = context;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_pixelShader = nullptr;
}

bool Engine::DirectX_Shader::SetAsActive()
{
	if (HasVertexShader() && HasPixelShader())
	{
		m_context->IASetInputLayout(m_vertexLayout.Get());
		m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
		m_context->PSSetShader(m_pixelShader.Get(), 0, 0);
		if (HasGeometryShader())
			m_context->GSSetShader(m_geometryShader.Get(), 0, 0);
		else
			m_context->GSSetShader(NULL, 0, 0);

		return true;
	}
	return false;
}

bool Engine::DirectX_Shader::SetVertexShader(const std::string & vsPath)
{
	HRESULT result;
	ComPtr<ID3DBlob> vsBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> vertexlayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexshader = nullptr;
	std::string error;
	auto vLayout = GetInputLayoutDescFromVertexLayout();
	result = CompileShader(To_wstr(vsPath).c_str(), "main", "vs_5_0", vsBlob, &error);
	if (FAILED(result))
	{
		OpenDialog(L"Load Error!", To_wstr(std::string("Could not open file: ") + vsPath + "\n\nOutput:\n" + error).c_str());
		return false;
	}

	result = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexshader);
	if (FAILED(result))
	{
		OpenDialog(L"CreateVertexShader failed!", To_wstr(std::string("Owner: ") + vsPath).c_str());
		return false;
	}

	result = m_device->CreateInputLayout(vLayout.data(), (uint32_t)vLayout.size(), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &vertexlayout);
	if (FAILED(result))
	{
		OpenDialog(L"CreatInputLayout failed!", To_wstr(std::string("Owner: ") + vsPath).c_str());
		return false;
	}

	ClearVertexShader();
	m_vertexLayout = vertexlayout;
	m_vertexShader = vertexshader;
	m_vsPath = vsPath;
	return true;
}

bool Engine::DirectX_Shader::SetPixelShader(const std::string & psPath)
{
	HRESULT result;
	ComPtr<ID3DBlob> psBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelshader = nullptr;
	std::string error;
	result = CompileShader(To_wstr(psPath).c_str(), "main", "ps_5_0", psBlob, &error);
	if (FAILED(result))
	{
		OpenDialog(L"Load Error!", To_wstr(std::string("Could not open file: ") + psPath + "\n\nOutput:\n" + error).c_str());
		return false;
	}

	result = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelshader);
	if (FAILED(result))
	{
		OpenDialog(L"CreatePixelShader failed!", To_wstr(std::string("Owner: ") + psPath).c_str());
		return false;
	}

	ClearPixelShader();
	m_pixelShader = pixelshader;
	m_psPath = psPath;
	return true;
}

bool Engine::DirectX_Shader::SetGeometryShader(const std::string & gsPath)
{
	HRESULT result;
	ComPtr<ID3DBlob> gsBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryshader = nullptr;
	std::string error;
	result = CompileShader(To_wstr(gsPath).c_str(), "main", "ps_5_0", gsBlob, &error);
	if (FAILED(result))
	{
		OpenDialog(L"Load Error!", To_wstr(std::string("Could not open file: ") + gsPath + "\n\nOutput:\n" + error).c_str());
		return false;
	}

	result = m_device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), NULL, &geometryshader);
	if (FAILED(result))
	{
		OpenDialog(L"CreateGeometryShader failed!", To_wstr(std::string("Owner: ") + gsPath).c_str());
		return false;
	}

	ClearGeometryShader();
	m_geometryShader = geometryshader;
	m_gsPath = gsPath;
	return true;
}

bool Engine::DirectX_Shader::HasVertexShader() const
{
	return m_vertexShader;
}

bool Engine::DirectX_Shader::HasPixelShader() const
{
	return m_pixelShader;
}

bool Engine::DirectX_Shader::HasGeometryShader() const
{
	return m_geometryShader;
}

std::string Engine::DirectX_Shader::GetVertexShaderPath() const
{
	return m_vsPath;
}

std::string Engine::DirectX_Shader::GetPixelShaderPath() const
{
	return m_psPath;
}

std::string Engine::DirectX_Shader::GetGeometryShaderPath() const
{
	return m_gsPath;
}

void Engine::DirectX_Shader::ClearVertexShader()
{
	m_vertexShader.Reset();
	m_vertexLayout.Reset();
	m_vsPath.clear();
}

void Engine::DirectX_Shader::ClearPixelShader()
{
	m_pixelShader.Reset();
	m_psPath.clear();
}

void Engine::DirectX_Shader::ClearGeometryShader()
{
	m_geometryShader.Reset();
	m_gsPath.clear();
}

std::vector<D3D11_INPUT_ELEMENT_DESC> Engine::DirectX_Shader::GetInputLayoutDescFromVertexLayout()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
	UINT byteOffset = 0;
	for (size_t index = 0; index < VertexInfo::NumElements(); index++)
	{
		size_t bytes = VertexInfo::SizeOf(index);
		DType type = VertexInfo::TypeOf(index);
		const std::string* name = VertexInfo::NameOf(index);
		D3D11_INPUT_ELEMENT_DESC desc = {};
		desc.AlignedByteOffset = byteOffset;
		desc.SemanticName = name->c_str();
		desc.SemanticIndex = 0;
		desc.InputSlot = 0;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		switch (type)
		{
		case Engine::DType::FLOAT:
			switch (bytes)
			{
			case 4:
				desc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
			case 8:
				desc.Format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case 12:
				desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case 16:
				desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			default:
				OpenDialog(L"Vertex Layout Set-up failed!", (std::wstring(L"Element is not 4 byte aligned.") +
					std::wstring(L"\nBytes: ") + std::to_wstring(bytes) +
					std::wstring(L"\nOwner: ") + To_wstr(*name)).c_str());
				throw std::exception();
				break;
			}
			break;
		case Engine::DType::UINT:
			switch (bytes)
			{
			case 4:
				desc.Format = DXGI_FORMAT_R32_UINT;
				break;
			case 8:
				desc.Format = DXGI_FORMAT_R32G32_UINT;
				break;
			case 12:
				desc.Format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case 16:
				desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			default:
				OpenDialog(L"Vertex Layout Set-up failed!", (std::wstring(L"Element is not 4 byte aligned.") +
					std::wstring(L"\nBytes: ") + std::to_wstring(bytes) +
					std::wstring(L"\nOwner: ") + To_wstr(*name)).c_str());
				throw std::exception();
				break;
			}
			break;
		default:
			OpenDialog(L"Vertex Layout Set-up failed!", (std::wstring(L"Invald data type.") +
				std::wstring(L"\nOwner: ") + To_wstr(*name)).c_str());
			throw std::exception();
			break;
		}

		layout.push_back(desc);
		byteOffset += (UINT)bytes;
	}
	return layout;
}

HRESULT Engine::DirectX_Shader::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint,
	LPCSTR profile, ComPtr<ID3DBlob>& blob, std::string* error)
{
	if (!srcFile || !entryPoint || !profile)
		return E_INVALIDARG;

	uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	ComPtr<ID3DBlob> shaderBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob && error)
		{
			const char* str = (char*)errorBlob->GetBufferPointer();
			*error = str;
		}

		return hr;
	}

	blob = shaderBlob;
	return hr;
}