#include "DirectX_Shader.h"
#include "Vertex.h"
#include <string>
#include <fstream>
#include <sstream>

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

bool Engine::DirectX_Shader::SetVertexShader(const std::string & vsPath, std::string* error)
{
	HRESULT result;
	bool value = true;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> vertexlayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexshader = nullptr;
	auto vLayout = GetInputLayoutDescFromVertexLayout(*error);
	if (vLayout.empty())
		return false;

	if (IsCompiled(vsPath))
	{
		std::vector<byte> bytes;
		int size = ReadBytes(vsPath.c_str(), bytes);
		if (!size)
		{
			if (error)
			{
				*error += "Failed to open " + vsPath + "\n";
			}
			value =  false;
		}
		else
		{
			result = m_device->CreateVertexShader((void*)&bytes[0], size, NULL, &vertexshader);
			if (FAILED(result))
			{
				_com_error err(result);
				std::wstring errMsg = err.ErrorMessage();
				if (error)
				{
					*error += "Could not create vertex shader from bytecode for file at " + vsPath + "\n" + To_str(errMsg) + "\n";
				}
				value =  false;
			}
			else
			{
				result = m_device->CreateInputLayout(vLayout.data(), (uint32_t)vLayout.size(), (void*)&bytes[0], size, &vertexlayout);
				if (FAILED(result))
				{
					if (error)
					{
						*error += "CreatInputLayout failed for file at " + vsPath + "\n";
					}
					value  = false;
				}
			}
		}
	}
	else
	{
		std::string compileError;
		ComPtr<ID3DBlob> vsBlob = nullptr;
		result = CompileShader(To_wstr(vsPath).c_str(), "main", "vs_5_0", vsBlob, &compileError);
		if (FAILED(result))
		{
			if (error)
			{
				*error += "Failed to open " + vsPath + "\n" + compileError + "\n";
			}
			value = false;
		}
		else
		{
			result = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexshader);
			if (FAILED(result))
			{
				if (error)
				{
					*error += "Could not create vertex shader from bytecode for file at " + vsPath + "\n";
				}
				value = false;
			}
			else
			{
				result = m_device->CreateInputLayout(vLayout.data(), (uint32_t)vLayout.size(), vsBlob->GetBufferPointer(),
					vsBlob->GetBufferSize(), &vertexlayout);
				if (FAILED(result))
				{
					if (error)
					{
						*error += "CreatInputLayout failed for file at " + vsPath + "\n";
					}
					value = false;
				}
			}
		}
	}

	ClearVertexShader();
	m_vertexLayout = vertexlayout;
	m_vertexShader = vertexshader;
	m_vsPath = vsPath;
	return value;
}

bool Engine::DirectX_Shader::SetPixelShader(const std::string & psPath, std::string* error)
{
	HRESULT result;
	bool value = true;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelshader = nullptr;

	if (IsCompiled(psPath))
	{
		std::vector<byte> bytes;
		int size = ReadBytes(psPath.c_str(), bytes);
		if (!size)
		{
			if (error)
			{
				*error += "Failed to open " + psPath + "\n";
			}
			value = false;
		}
		else
		{
			result = m_device->CreatePixelShader((void*)&bytes[0], size, NULL, &pixelshader);
			if (FAILED(result))
			{
				if (error)
				{
					*error += "Could not create pixel shader from bytecode for file at " + psPath + "\n";
				}
				value = false;
			}
		}
	}
	else
	{
		std::string compileError;
		ComPtr<ID3DBlob> psBlob = nullptr;
		result = CompileShader(To_wstr(psPath).c_str(), "main", "ps_5_0", psBlob, &compileError);
		if (FAILED(result))
		{
			if (error)
			{
				*error += "Failed to open " + psPath + "\n" + compileError + "\n";
			}
			value = false;
		}
		else
		{

			result = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelshader);
			if (FAILED(result))
			{
				if (error)
				{
					*error += "Could not create pixel shader from compiled file " + psPath + "\n";
				}
				value = false;
			}
		}
	}

	ClearPixelShader();
	m_pixelShader = pixelshader;
	m_psPath = psPath;
	return value;
}

bool Engine::DirectX_Shader::SetGeometryShader(const std::string & gsPath, std::string* error)
{
	HRESULT result;
	bool value = true;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryshader = nullptr;

	if (IsCompiled(gsPath))
	{
		std::vector<byte> bytes;
		int size = ReadBytes(gsPath.c_str(), bytes);
		if (!size)
		{
			if (error)
			{
				*error += "Failed to open " + gsPath + "\n";
			}
			value = false;
		}
		else
		{
			result = m_device->CreateGeometryShader((void*)&bytes[0], size, NULL, &geometryshader);
			if (FAILED(result))
			{
				if (error)
				{
					*error += "Could not create geometry shader from bytecode for file at " + gsPath + "\n";
				}
				value = false;
			}
		}
	}
	else
	{
		std::string compileError;
		ComPtr<ID3DBlob> gsBlob = nullptr;
		result = CompileShader(To_wstr(gsPath).c_str(), "main", "ps_5_0", gsBlob, &compileError);
		if (FAILED(result))
		{
			if (error)
			{
				*error += "Failed to open " + gsPath + "\n" + compileError + "\n";
			}
			value = false;
		}
		else
		{
			result = m_device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), NULL, &geometryshader);
			if (FAILED(result))
			{
				if (error)
				{
					*error += "Could not create geometry shader from bytecode for file at " + gsPath + "\n";
				}
				value = false;
			}
		}
	}

	ClearGeometryShader();
	m_geometryShader = geometryshader;
	m_gsPath = gsPath;
	return value;
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

std::vector<D3D11_INPUT_ELEMENT_DESC> Engine::DirectX_Shader::GetInputLayoutDescFromVertexLayout(std::string& retError)
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
				retError += "Vertex Layout Set-up failed! Element is not 4 byte aligned.\nBytes : " 
					+ std::to_string(bytes) + " read for " + *name;
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
				retError += "Vertex Layout Set-up failed! Element is not 4 byte aligned.\nBytes : "
					+ std::to_string(bytes) + " read for " + *name;
				throw std::exception();
				break;
			}
			break;
		default:
			retError += "Vertex Layout Set-up failed! Invald data type given.";
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

int Engine::DirectX_Shader::ReadBytes(const std::string& name, std::vector<byte>& bytes)
{
	bytes.clear();
	std::ifstream infile(name, std::ios::in | std::ios::binary);
	if (infile)
	{

		//get length of file
		infile.seekg(0, infile.end);
		size_t length = infile.tellg();
		infile.seekg(0, infile.beg);
		bytes.resize(length, 0);

		//read file
		infile.read((char*)&bytes[0], length);
	}
	return (int)bytes.size();
}

bool Engine::DirectX_Shader::IsCompiled(const std::string & filepath)
{
	std::string ext = filepath.substr(filepath.find_last_of('.'), std::string::npos);
	if (!ext.compare(COMPILED_SHADER_EXT))
		return true;

	return false;
}
