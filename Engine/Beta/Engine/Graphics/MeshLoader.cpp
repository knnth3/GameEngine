#include "MeshLoader.h"
#include "WindowsAdditionals.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <BMImporter\BinImporter.h>


using namespace Graphics;
using namespace std;
bool Graphics::MeshLoader::m_bIsInit = false;
std::shared_ptr<Mesh> Graphics::MeshLoader::m_default;
std::vector<std::shared_ptr<Mesh>> Graphics::MeshLoader::m_modelLibrary;
std::map<std::string, MeshID> Graphics::MeshLoader::m_filepaths;
std::map<std::string, MeshID> Graphics::MeshLoader::m_keyNames;

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
	BMImporter io;
	MeshData meshdata;
	if (io.Import(filename, meshdata))
	{
		auto data = std::make_shared<Mesh>();
		for (auto i : meshdata.m_indices)
		{
			data->m_indices.push_back(Index(i));
		}

		for (auto v : meshdata.m_vertices)
		{
			Vertex newVertex;

			newVertex.m_position.x = v.m_position.x;
			newVertex.m_position.y = v.m_position.y;
			newVertex.m_position.z = v.m_position.z;

			newVertex.m_normal.x = v.m_normal.x;
			newVertex.m_normal.y = v.m_normal.y;
			newVertex.m_normal.z = v.m_normal.z;

			if (v.m_bHasTangents)
			{
				newVertex.m_uv.x = v.m_uv.x;
				newVertex.m_uv.y = v.m_uv.y;

				newVertex.m_tangent.x = v.m_tangent.x;
				newVertex.m_tangent.y = v.m_tangent.y;
				newVertex.m_tangent.z = v.m_tangent.z;

				newVertex.m_binormal.x = v.m_binormal.x;
				newVertex.m_binormal.y = v.m_binormal.y;
				newVertex.m_binormal.z = v.m_binormal.z;
			}
			else
			{
				data->m_bUsingVertexColors = true;
				newVertex.m_color.x = v.m_color.x;
				newVertex.m_color.y = v.m_color.y;
				newVertex.m_color.z = v.m_color.z;
			}

			data->m_vertices.push_back(newVertex);
		}

		result = SaveMesh(data);
	}
	return result;
}

Graphics::MeshID Graphics::MeshLoader::SaveMesh(const std::shared_ptr<Mesh>& mesh)
{
	MeshID result = -1;
	size_t models = m_modelLibrary.size();

	result = (MeshID)models;
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