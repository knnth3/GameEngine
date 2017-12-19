#include "MeshLoader.h"
#include "WindowsAdditionals.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>


using namespace Graphics;
using namespace std;
bool Graphics::MeshLoader::m_bIsInit = false;
std::shared_ptr<Mesh> Graphics::MeshLoader::m_default;
std::vector<std::shared_ptr<Mesh>> Graphics::MeshLoader::m_modelLibrary;
std::map<std::string, MeshID> Graphics::MeshLoader::m_filepaths;
std::map<std::string, MeshID> Graphics::MeshLoader::m_keyNames;

template<typename T>
bool Graphics::Vector3<T>::operator==(const Vector3<T>& v)
{
	if (this->x == v.x && this->y == v.y && this->z == v.z)
	{
		return true;
	}
	return false;
}

template<typename T>
glm::vec3 Graphics::Vector3<T>::to_glm()
{
	return glm::vec3((float)this->x, (float)this->y, (float)this->z);
}

bool Graphics::VertexData::operator==(const VertexData & v)
{
	auto pos = this->m_position == v.m_position;
	auto normals = this->m_normal == v.m_normal;
	auto tangents = this->m_tangent == v.m_tangent;
	auto bitangents = this->m_binormal == v.m_binormal;
	auto textureCoords = this->m_uv == v.m_uv;

	if (pos && normals && tangents && bitangents && textureCoords)
	{
		return true;
	}
	return false;
}

bool Graphics::MeshLoader::Initialize()
{
	auto defaultMesh = MeshLoader::LoadModel("Assets/Models/Cube.bin");
	if (defaultMesh == -1)
		return false;

	m_default = m_modelLibrary[defaultMesh];
	Clear();
	m_bIsInit = true;
	return true;
}

Graphics::MeshID Graphics::MeshLoader::LoadModel(const std::string filename)
{
	MeshID result = -1;
	std::string ext;
	GetFileExt(filename, ext);

	struct stat results;
	if (!stat(filename.c_str(), &results) == 0)
	{
		std::wstring message;
		message.insert(message.end(), filename.begin(), filename.end());
		CheckSuccess(E_INVALIDARG, message.c_str());
		return result;
	}

	if (!IsFilepathQuerried(filename, result))
	{
		result = CreateMesh(filename);
	}
	return result;
}

Graphics::MeshID Graphics::MeshLoader::LoadModel(const std::vector<Vertex>& verts, const std::vector<Index>& indices, const std::string uniqueName)
{
	MeshID result = -1;
	//if(!IsKeyNameQuerried(uniqueName, result))
	//{
	//	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	//	MeshDefaultSettings settings = {};

	//}

	return result;
}

MeshID Graphics::MeshLoader::CreatePlane(float xUnits, float zUnits, int xTesselation, int zTesselation)
{
	auto data = std::make_shared<Mesh>();

	float offsetX = xUnits * xTesselation * 0.5f;
	float offsetZ = zUnits * zTesselation * 0.5f;

	Vertex basic;
	basic.m_normal = { 0.0f, 1.0f, 0.0f, 0.0f };
	basic.m_binormal = { 0.0f, 1.0f, 0.0f };
	basic.m_tangent = { 0.0f, 1.0f, 0.0f };

	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec2> m_textureCoords;

	m_positions.push_back({ -offsetX          , 0.0f, -offsetZ          });
	m_positions.push_back({ -offsetX          , 0.0f, -offsetZ + zUnits });
	m_positions.push_back({ -offsetX + xUnits , 0.0f, -offsetZ + zUnits });
	m_positions.push_back({ -offsetX + xUnits , 0.0f, -offsetZ + zUnits });
	m_positions.push_back({ -offsetX + xUnits , 0.0f, -offsetZ          });
	m_positions.push_back({ -offsetX          , 0.0f, -offsetZ          });

	m_textureCoords.push_back({ 0.0f, 0.0f });
	m_textureCoords.push_back({ 0.0f, 1.0f });
	m_textureCoords.push_back({ 1.0f, 1.0f });
	m_textureCoords.push_back({ 1.0f, 1.0f });
	m_textureCoords.push_back({ 1.0f, 0.0f });
	m_textureCoords.push_back({ 0.0f, 0.0f });

	Index count = 0;
	for (int x = 0; x < xTesselation; x++)
	{
		for (int z = 0; z < zTesselation; z++)
		{
			for (size_t it = 0; it < m_positions.size(); it++)
			{
				basic.m_uv = m_textureCoords[it];
				basic.m_position = m_positions[it];

				basic.m_position.x += (float)x * xUnits;
				basic.m_position.z += (float)z * zUnits;

				data->m_vertices.push_back(basic);
				data->m_indices.push_back(count++);
			}
		}
	}

	return SaveMesh(data);
}

void Graphics::MeshLoader::Clear()
{
	m_modelLibrary.clear();
	m_filepaths.clear();
	m_keyNames.clear();
}

void Graphics::MeshLoader::GrabMeshData(MeshID id, std::shared_ptr<Mesh> & ptr)
{
	if (m_modelLibrary.size() > id && id >= 0)
		ptr = m_modelLibrary.at(id);
	else
		ptr = m_default;
}

Graphics::MeshID Graphics::MeshLoader::CreateMesh(const std::string filename)
{
	MeshID result = -1;
	auto data = std::make_shared<Mesh>();
	unsigned int index;
	Vertex newVertex;
	VertexData vertData;

	int indexSize = 0;
	int vertexSize = 0;
	ifstream myFile(filename.c_str(), ios::in | ios::binary);
	if (myFile)
	{
		if (!myFile.read((char*)&indexSize, sizeof(int)))
			return result;

		if (!myFile.read((char*)&vertexSize, sizeof(int)))
			return result;

		for (int it = 0; it < indexSize; it++)
		{
			if (!myFile.read((char*)&index, sizeof(unsigned int)))
				return result;

			data->m_indices.push_back((Index)index);
		}

		for (int it = 0; it < vertexSize; it++)
		{
			if (!myFile.read((char*)&vertData, sizeof(VertexData)))
				return result;

			newVertex.m_position = vertData.m_position.to_glm();
			newVertex.m_normal = glm::vec4(vertData.m_normal.to_glm(), 1.0f);
			newVertex.m_tangent = vertData.m_tangent.to_glm();
			newVertex.m_binormal = vertData.m_binormal.to_glm();
			newVertex.m_uv = vertData.m_uv.to_glm();

			data->m_vertices.push_back(newVertex);
		}

		myFile.close();
		result = SaveMesh(data);
	}
	return result;
}

Graphics::MeshID Graphics::MeshLoader::SaveMesh(const std::shared_ptr<Mesh>& mesh)
{
	MeshID result = -1;
	size_t models = m_modelLibrary.size();

	result = (MeshID)models;
	mesh->objectID = result;
	m_modelLibrary.push_back(mesh);

	return result;
}

bool Graphics::MeshLoader::IsFilepathQuerried(const std::string filepath, MeshID & result)
{
	result = -1;
	if (!filepath.empty())
	{
		auto found = m_filepaths.find(filepath);
		if (found != m_filepaths.end())
		{
			result = found->second;
			return true;
		}
	}

	return false;
}

bool Graphics::MeshLoader::IsKeyNameQuerried(const std::string filepath, MeshID & result)
{
	result = -1;
	if (!filepath.empty())
	{
		auto found = m_keyNames.find(filepath);
		if (found != m_keyNames.end())
		{
			result = found->second;
			return true;
		}
	}

	return false;
}