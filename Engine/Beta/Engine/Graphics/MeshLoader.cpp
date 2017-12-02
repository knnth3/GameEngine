#include "MeshLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader\tiny_obj_loader.h>


using namespace Graphics;
using namespace std;
bool Graphics::MeshLoader::m_bIsInit = false;
std::shared_ptr<Mesh> Graphics::MeshLoader::m_default;
std::vector<std::shared_ptr<Mesh>> Graphics::MeshLoader::m_modelLibrary;
std::vector<MeshDefaultSettings> Graphics::MeshLoader::m_defaultSettings;
std::map<std::string, MeshID> Graphics::MeshLoader::m_filepaths;
std::map<std::string, MeshID> Graphics::MeshLoader::m_keyNames;


bool Graphics::MeshLoader::Initialize()
{
	auto defaultMesh = MeshLoader::LoadModel("Assets/Models/Cube.obj");
	if (defaultMesh == -1)
		return false;

	m_default = m_modelLibrary[defaultMesh];
	Clear();
	m_bIsInit = true;
	return true;
}

Graphics::MeshID Graphics::MeshLoader::LoadModel(const std::string filename)
{
	MeshID result;
	if (!IsFilepathQuerried(filename, result))
	{
		std::string ext;
		GetFileExt(filename, ext);

		if(ext.compare("obj") == 0)
			result = ProcessOBJFile(filename);
	}

	return result;
}

Graphics::MeshID Graphics::MeshLoader::LoadModel(const std::vector<Vertex>& verts, const std::vector<Index>& indices, const std::string uniqueName)
{
	MeshID result;
	if(!IsKeyNameQuerried(uniqueName, result))
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		MeshDefaultSettings settings = {};
		settings.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		Polygon p;
		p.m_vertices.insert(p.m_vertices.end(), verts.begin(), verts.end());
		p.m_indices.insert(p.m_indices.end(), indices.begin(), indices.end());
		mesh->m_polygons.push_back(p);

		result = SaveInformation(mesh, settings);
		m_keyNames.emplace(uniqueName, result);
	}

	return result;
}

Graphics::MeshID Graphics::MeshLoader::CreateLine(glm::vec3 pos1, glm::vec3 pos2)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	MeshDefaultSettings settings = {};
	settings.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	Polygon poly;
	poly.m_vertices = 
	{
			{ pos1, glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) },
			{ pos2, glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) },
	};
	poly.m_indices =
	{
		0,1
	};
	mesh->m_polygons.push_back(poly);

	return SaveInformation(mesh, settings);
}

void Graphics::MeshLoader::Clear()
{
	m_modelLibrary.clear();
	m_defaultSettings.clear();
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

void Graphics::MeshLoader::GetDefaulMeshInfo(MeshID id, MeshDefaultSettings& settings)
{
	if(m_defaultSettings.size() > id)
		settings = m_defaultSettings.at(id);
}

Graphics::MeshID Graphics::MeshLoader::SaveInformation(const std::shared_ptr<Mesh> & data, const MeshDefaultSettings & settings)
{
	return SaveMesh(data, settings);
}

Graphics::MeshID Graphics::MeshLoader::ProcessOBJFile(const std::string filename)
{
	Graphics::MeshID result = -1;
	auto mesh = std::make_shared<Mesh>();
	MeshDefaultSettings settings = {};
	settings.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	if (CreateMeshFromOBJ(filename, mesh, settings))
	{
		result = SaveInformation(mesh, settings);
		m_filepaths.emplace(filename, result);
	}

	return result;
}

bool Graphics::MeshLoader::CreateMeshFromOBJ(const std::string filename, std::shared_ptr<Mesh>& data, MeshDefaultSettings & settings)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());

	//Can contain warnings as well
	if (!err.empty())
	{

#if PLATFORM == OS_WINDOWS
		wstring msg = wstring(err.begin(), err.end());
		MessageBox(NULL, msg.c_str(), L"Error", MB_ICONEXCLAMATION);
#endif

	}

	if (!ret)
		return false;

	//Shapes contains all objects in scene
	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		//Go through all faces of the shape
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			Polygon poly;
			int fv = shapes[s].mesh.num_face_vertices[f];

			//Go through all vertices in face
			for (size_t v = 0; v < fv; v++)
			{
				//Prefix definitions:
				//	tinyobj::real_t = float
				//	v = pos coord
				//	n = normal
				//	t = texture coord
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				//Make sure the obj file contains the correct perams
				if (idx.normal_index < 0 || idx.texcoord_index < 0 || idx.vertex_index < 0)
					return false;

				Vertex vert;
				vert.m_position.x = attrib.vertices[3 * idx.vertex_index + 0];
				vert.m_position.y = attrib.vertices[3 * idx.vertex_index + 1];
				vert.m_position.z = attrib.vertices[3 * idx.vertex_index + 2];

				vert.m_normal.x = attrib.normals[3 * idx.normal_index + 0];
				vert.m_normal.y = attrib.normals[3 * idx.normal_index + 1];
				vert.m_normal.z = attrib.normals[3 * idx.normal_index + 2];

				vert.m_uv.x = attrib.texcoords[2 * idx.texcoord_index + 0];
				vert.m_uv.y = attrib.texcoords[2 * idx.texcoord_index + 1];

				poly.m_vertices.push_back(vert);
				poly.m_indices.push_back(Graphics::Index(index_offset + v));
			}
			index_offset += fv;
			data->m_polygons.push_back(poly);
		}

	}
	return true;
}

Graphics::MeshID Graphics::MeshLoader::SaveMesh(const std::shared_ptr<Mesh>& mesh, const MeshDefaultSettings & setting)
{
	MeshID result = -1;
	size_t models = m_modelLibrary.size();
	size_t settings = m_defaultSettings.size();
	if (models == settings)
	{
		result = (MeshID)models;
		mesh->objectID = result;
		m_modelLibrary.push_back(mesh);
		m_defaultSettings.push_back(setting);
	}

	//TODO:
	//Output log information that MeshLibrary is corrupt
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