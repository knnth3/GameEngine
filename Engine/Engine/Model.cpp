#include <glm\gtx\transform.hpp>
#include "Model.h"

using namespace Lime::Model;
using namespace std;

static MeshLoader MESHLIB;

c_uint Lime::Model::Polygon::GetVertexCount()
{
	return (c_uint)m_vertices.size();
}

c_uint Lime::Model::Polygon::GetIndexCount()
{
	return (c_uint)m_indices.size();
}

void Lime::Model::MeshData::GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const
{
	for (size_t x = 0; x < polygons.size(); x++)
	{
		auto& index = polygons.at(x).m_indices;
		indices.insert(indices.end(), index.begin(), index.end());
	}
	for (size_t x = 0; x < m_vertices.size(); x++)
	{
		auto& vertex = m_vertices.at(x);
		verts.push_back(*vertex);
	}
}

uint32_t Lime::Model::MeshData::GetIndexCount()
{
	return m_indexCount;
}

Lime::Model::Model3D::Model3D()
{
	m_outRotation = glm::mat4();
	m_inRotation = glm::mat4();
	m_scaleMatrix = glm::mat4();
	m_translation = glm::mat4();
	m_localToWorld = glm::mat4();
	m_position = glm::vec3();
	m_offset = glm::vec3();
	m_texture = 0;
	m_mesh = nullptr;
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Lime::Model::Model3D::Model3D(const MeshID id) :
	Model3D()
{
	AddMesh(id);
}

void Lime::Model::Model3D::Scale(const float x, const float y, const float z)
{
	m_scale = glm::vec3(x, y, z);
	m_offset.x *= m_scale.x;
	m_offset.y *= m_scale.y;
	m_offset.z *= m_scale.z;
	m_scaleMatrix = glm::scale(m_scale);
}

void Lime::Model::Model3D::Scale(glm::vec3 scale)
{
	m_scale = scale;
}

void Lime::Model::Model3D::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Lime::Model::Model3D::SetPosition(glm::vec3 pos)
{
	m_position = pos;
}

void Lime::Model::Model3D::Rotate(float x, float y, float z)
{
	RotateMatrix(m_outRotation, glm::vec3(x,y,z));
}

void Lime::Model::Model3D::Rotate(glm::vec3 rotation)
{
	RotateMatrix(m_outRotation, rotation);
}

void Lime::Model::Model3D::RotateAtOrigin(float x, float y, float z)
{
	RotateMatrix(m_inRotation, glm::vec3(x,y,z));
}

void Lime::Model::Model3D::SetColor(float r, float g, float b)
{
	m_color = glm::vec4(r, g, b, 1.0f);
}

void Lime::Model::Model3D::SetColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void Lime::Model::Model3D::SetColor(glm::vec4 color)
{
	m_color = color;
}

void Lime::Model::Model3D::SetOpacity(float alpha)
{
	m_color.a = alpha;
}

void Lime::Model::Model3D::SetOffset(float offset)
{
	m_offset = glm::vec3(offset * m_scale.x, 0.0f, 0.0f);
}

void Lime::Model::Model3D::SetTexture(Texture tex)
{
	m_texture = tex;
}

void Lime::Model::Model3D::AddMesh(MeshID id)
{
	MeshLoader::GrabMeshData(id, m_mesh);
	if (id > -1)
	{
		//Possibly implement this later
		//MeshLoader::SetDefaultSettings(id, *this);
		modelType = MESH;
	}
}

glm::vec3 Lime::Model::Model3D::GetPosition() const
{
	return m_position - m_offset;
}

glm::mat4 Lime::Model::Model3D::GetModelMatrix()
{
	CreateLocalToWorld();
	return m_localToWorld;
}

glm::vec4 Lime::Model::Model3D::GetColor()
{
	return m_color;
}

Texture Lime::Model::Model3D::GetTexture()
{
	return m_texture;
}

void Lime::Model::Model3D::operator=( MeshID id)
{
	AddMesh(id);
}

void Lime::Model::Model3D::CreateLocalToWorld()
{
	glm::vec3 pos = m_position - m_offset;
	m_translation = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));
	m_localToWorld = m_inRotation * m_translation * m_outRotation * m_scaleMatrix;
}

void Lime::Model::Model3D::RotateMatrix(glm::mat4 & matrix, glm::vec3 rot)
{
	matrix = glm::mat4();
	if (rot.x != 0.0f)
	{
		glm::vec3 rotaxis(1.0f, 0.0f, 0.0f);
		matrix = glm::rotate(matrix, rot.x, rotaxis);
	}
	if (rot.y != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 1.0f, 0.0f);
		matrix = glm::rotate(matrix, rot.y, rotaxis);
	}
	if (rot.z != 0.0f)
	{
		glm::vec3 rotaxis(0.0f, 0.0f, 1.0f);
		matrix = glm::rotate(matrix, rot.z, rotaxis);
	}
}

MeshID Lime::Model::MeshLoader::LoadModel(std::string filename, ModelType type)
{
	bool IsFBX = true;
	MeshData_ptr mesh = nullptr;
	//Creates a handle for memory management
	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	if (!lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings())) {
		IsFBX = false;
		lImporter->Destroy();
		lSdkManager->Destroy();
		return -1;
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	lImporter->Import(lScene);
	lImporter->Destroy();
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode)
	{
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			auto node = lRootNode->GetChild(i);
			ProcessElement(node, mesh, type);
			MeshDefaultSettings settings;
			settings.rotation = FbxToGlmVec(node->LclRotation.Get());
			settings.scale = FbxToGlmVec(node->LclScaling.Get());
			MESHLIB.m_defaultSettings.push_back(settings);
		}
	}
	lScene->Destroy();
	lSdkManager->Destroy();

	MeshID id = (MeshID)MESHLIB.m_modelLibrary.size();
	mesh->objectID = id;
	MESHLIB.m_modelLibrary.push_back(mesh);
	return id;
}

void Lime::Model::MeshLoader::GrabMeshData(MeshID id, MeshData_ptr & ptr)
{
	try
	{
		ptr = MESHLIB.m_modelLibrary.at(id);
	}
	catch (exception e)
	{
		ptr = std::make_shared<MeshData>();
	}
}

void Lime::Model::MeshLoader::SetDefaultSettings(MeshID id, Model3D & ptr)
{
	try
	{
		ptr.Scale(MESHLIB.m_defaultSettings.at(id).scale);
		ptr.SetPosition(MESHLIB.m_defaultSettings.at(id).translation);
		ptr.Rotate(MESHLIB.m_defaultSettings.at(id).rotation);
	}
	catch (exception e)
	{
		return;
	}
}

bool Lime::Model::MeshLoader::GrabMeshNormal(FbxMesh* mesh, Vertex& vert, int vertIndex)
{
	bool result = false;
	if (mesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
		//-Used in cases where object has per vertex normal
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			vert.m_normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertIndex).mData[0]);
			vert.m_normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertIndex).mData[1]);
			vert.m_normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertIndex).mData[2]);
			//Invert the output to work with DirectX coordinate system
			vert.m_normal = -vert.m_position;
			result = true;
		}
		break;
		}
		break;
		//-Used in cases where object has hard edges (per face normal)
		//-This is currently Ignored and per vertex normal is obtained
		// Use case FbxGeometryElement::eIndexToDirect: to access normals by index
		// and vertexNormal->GetIndexArray().GetAt(inVertexCounter);
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			vert.m_normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertIndex).mData[0]);
			vert.m_normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertIndex).mData[1]);
			vert.m_normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertIndex).mData[2]);
			//Invert the output to work with DirectX coordinate system
			vert.m_normal = -vert.m_position;
			result = true;
		}
		break;
		}
		break;
	}

	return result;
}

void Lime::Model::MeshLoader::ProcessElement(FbxNode * pNode, Lime::Model::MeshData_ptr& data, ModelType type)
{
	data = std::make_shared<MeshData>();
	data->m_indexCount = 0;
	FbxMesh* mesh = pNode->GetMesh();
	unsigned int vertexCount = mesh->GetControlPointsCount();
	unsigned int polygonCount = mesh->GetPolygonCount();

	for (uint32_t x = 0; x < vertexCount; x++)
	{
		Vertex vert;
		vert.m_position.x = static_cast<float>(mesh->GetControlPointAt(x).mData[0]);
		vert.m_position.y = static_cast<float>(mesh->GetControlPointAt(x).mData[1]);
		vert.m_position.z = static_cast<float>(mesh->GetControlPointAt(x).mData[2]);

		if (!GrabMeshNormal(mesh, vert, x))
			return;

		if (type == Model::TEXT && vertexCount == 4)
		{
			if (vert.m_position.x > 0.0f)
				vert.m_position.x = 0.6f;
			else
				vert.m_position.x = -0.6f;

			if (vert.m_position.y > 0.0f)
				vert.m_position.y = 1.0f;
			else
				vert.m_position.y = -1.0f;
		}

		data->m_vertices.emplace_back(new Vertex(vert));
	}
	if (type == Model::TEXT && vertexCount == 4)
	{
		data->m_vertices[0]->m_uv = glm::vec2(0.0f, 1.0f);
		data->m_vertices[1]->m_uv = glm::vec2(1.0f, 1.0f);
		data->m_vertices[2]->m_uv = glm::vec2(0.0f, 0.0f);
		data->m_vertices[3]->m_uv = glm::vec2(1.0f, 0.0f);

	}

	for (uint32_t x = 0; x < polygonCount; x++)
	{
		Polygon shape;
		int size = mesh->GetPolygonSize(x);
		for (int y = 0; y < size; y++)
		{
			int index = mesh->GetPolygonVertex(x, y);
			shape.m_indices.push_back(index);
			shape.m_vertices.push_back(data->m_vertices.at(index));
			data->m_indexCount++;
		}
		data->polygons.push_back(shape);
	}
}

glm::vec3 Lime::Model::MeshLoader::FbxToGlmVec(FbxDouble3 in)
{
	glm::vec3 output;
	output.x = (float)in.mData[0];
	output.y = (float)in.mData[1];
	output.z = (float)in.mData[2];
	return output;
}
