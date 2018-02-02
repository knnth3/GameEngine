#include "MeshLoader.h"
#include "WindowsAdditionals.h"
#include <sys/stat.h>
//#include <BinImporter\BinImporter.h>
#include <SEF\SEStream.h>


using namespace Engine;
using namespace std;
bool Engine::MeshLoader::m_bIsInit = false;
std::shared_ptr<Mesh> Engine::MeshLoader::m_default;
std::vector<std::shared_ptr<Mesh>> Engine::MeshLoader::m_modelLibrary;
std::map<std::string, int> Engine::MeshLoader::m_filepaths;
std::map<std::string, int> Engine::MeshLoader::m_keyNames;

bool Engine::MeshLoader::Initialize(const std::string& defaultMesh)
{
	auto id = CreateMesh(defaultMesh);
	if (id == -1)
		return false;

	m_default = m_modelLibrary[id];
	Clear();
	m_bIsInit = true;
	return true;
}

int Engine::MeshLoader::LoadModel(const std::string& filename)
{
	int result = -1;
	if (CheckInit(filename))
	{
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
	}
	return result;
}

int Engine::MeshLoader::LoadModel(const std::vector<Vertex>& verts, const std::vector<Index>& indices, const std::string& uniqueName)
{
	int result = -1;
	//if(!IsKeyNameQuerried(uniqueName, result))
	//{
	//	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	//	MeshDefaultSettings settings = {};

	//}

	return result;
}

int Engine::MeshLoader::CreatePlane(float xUnits, float zUnits, int xTesselation, int zTesselation)
{
	if (CheckInit("Plane Create Method"))
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

		m_positions.push_back({ -offsetX          , 0.0f, -offsetZ });
		m_positions.push_back({ -offsetX          , 0.0f, -offsetZ + zUnits });
		m_positions.push_back({ -offsetX + xUnits , 0.0f, -offsetZ + zUnits });
		m_positions.push_back({ -offsetX + xUnits , 0.0f, -offsetZ + zUnits });
		m_positions.push_back({ -offsetX + xUnits , 0.0f, -offsetZ });
		m_positions.push_back({ -offsetX          , 0.0f, -offsetZ });

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

					data->Vertices.push_back(basic);
					data->Indices.push_back(count++);
				}
			}
		}

		return SaveMesh(data);
	}
	return -1;
}

void Engine::MeshLoader::Clear()
{
	m_modelLibrary.clear();
	m_filepaths.clear();
	m_keyNames.clear();
}

void Engine::MeshLoader::GrabMeshData(int id, std::shared_ptr<Mesh> & ptr)
{
	if (m_modelLibrary.size() > id && id >= 0)
		ptr = m_modelLibrary.at(id);
	else
		ptr = m_default;
}

int Engine::MeshLoader::CreateMesh(const std::string& filename)
{
	int result = -1;
	SEStream io;
	MeshData meshdata;
	Skeleton skeldata;
	if (io.ReadFile(filename, &meshdata, &skeldata))
	{
		auto data = std::make_shared<Mesh>();
		for (auto i : meshdata.Indices)
		{
			data->Indices.push_back(Index(i));
		}

		for (auto v : meshdata.Vertices)
		{
			Vertex newVertex;

			newVertex.m_position.x = v.Position.x;
			newVertex.m_position.y = v.Position.y;
			newVertex.m_position.z = v.Position.z;

			newVertex.m_normal.x = v.Normal.x;
			newVertex.m_normal.y = v.Normal.y;
			newVertex.m_normal.z = v.Normal.z;

			if (v.bHasUV)
			{
				newVertex.m_uv.x = v.UV.x;
				newVertex.m_uv.y = v.UV.y;

				newVertex.m_tangent.x = v.Tangent.x;
				newVertex.m_tangent.y = v.Tangent.y;
				newVertex.m_tangent.z = v.Tangent.z;

				newVertex.m_binormal.x = v.Binormal.x;
				newVertex.m_binormal.y = v.Binormal.y;
				newVertex.m_binormal.z = v.Binormal.z;
			}
			else
			{
				data->CreationFlags = CREATION_TYPE_NO_UV;
				newVertex.m_color.x = v.Color.x;
				newVertex.m_color.y = v.Color.y;
				newVertex.m_color.z = v.Color.z;
			}

			data->Vertices.push_back(newVertex);
		}

		result = SaveMesh(data);
	}
	return result;
}

int Engine::MeshLoader::SaveMesh(const std::shared_ptr<Mesh>& mesh)
{
	int result = -1;
	size_t models = m_modelLibrary.size();

	result = (int)models;
	m_modelLibrary.push_back(mesh);

	return result;
}

bool Engine::MeshLoader::IsFilepathQuerried(const std::string& filepath, int result)
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

bool Engine::MeshLoader::IsKeyNameQuerried(const std::string& filepath, int result)
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

bool Engine::MeshLoader::CheckInit(const std::string& filename)
{
	wstring error;
	if (!m_bIsInit)
	{
		error = L"Attempted to load a mesh before initialization. \nDo not load a mesh before App's resume function.";
		error += L"\nFile: " + To_wstr(filename);
		OpenDialog(L"MeshLoader Error!", error.c_str());
	}
	return m_bIsInit;
}
