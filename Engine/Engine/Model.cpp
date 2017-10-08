#include <glm\gtx\transform.hpp>
#include <typeinfo> 
#include "Model.h"

//-Checks a functions return value agains desired result
//-does whatever instructed to do on fail;
#define ENFORCE_SUCCESS(function, desiredResult, whatToDoOnFail) \
		if (!desiredResult == function) { whatToDoOnFail; }

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF(fbxManager) (*(fbxManager->GetIOSettings()))
#endif

using namespace Lime::Model;
using namespace std;

static MeshLibrary MESHLIB;


bool Lime::Model::VertexLibrary::empty()
{
	size_t vert = m_vertices.size();
	size_t ind = m_indices.size();

	size_t result = vert*ind;
	if (result == 0)
		return true;

	return false;
}

void Lime::Model::VertexLibrary::clear()
{
	m_cachedIDs.clear();
	m_vertices.clear();
	m_indices.clear();
	m_models.clear();
}

size_t Lime::Model::VertexLibrary::size()
{
	return m_models.size();
}

void Lime::Model::VertexLibrary::AddModel(std::shared_ptr<Model::Model3D>& model)
{
	bool similar = false;
	for (auto const& id : m_cachedIDs)
	{
		if (id == model->m_mesh->objectID)
		{
			similar = true;
		}
	}
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	model->m_mesh->GetBuffers(vertices, indices);
	if (!similar)
	{
		m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
		m_indices.insert(m_indices.end(), indices.begin(), indices.end());

		model->m_mesh->vertOffset = VertCountOffset;
		model->m_mesh->indiciOffset = IndCountOffset;
		VertCountOffset += (int)vertices.size();
		IndCountOffset += (int)indices.size();
		m_cachedIDs.push_back(model->m_mesh->objectID);
	}

	m_models.push_back(model);
}

const void * Lime::Model::VertexLibrary::VertexData()
{
	return m_vertices.data();
}

const void * Lime::Model::VertexLibrary::IndexData()
{
	return m_indices.data();
}

uint32_t Lime::Model::VertexLibrary::VertexDataSize()
{
	return (uint32_t)m_vertices.size();
}

uint32_t Lime::Model::VertexLibrary::IndexDataSize()
{
	return (uint32_t)m_indices.size();
}

std::shared_ptr<Lime::Model::Model3D>& Lime::Model::VertexLibrary::operator[](const size_t index)
{
	return m_models[index];
}

template<class T>
c_uint Lime::Model::Polygon<T>::GetVertexCount()
{
	return (c_uint)m_vertices.size();
}
template<class T>
c_uint Lime::Model::Polygon<T>::GetIndexCount()
{
	return (c_uint)m_indices.size();
}

void Lime::Model::MeshData::GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const
{
	verts.clear();
	indices.clear();

	for (size_t x = 0; x < m_polygons.size(); x++)
	{
		verts.insert(verts.end(), m_polygons[x].m_vertices.begin(), m_polygons[x].m_vertices.end());
		indices.insert(indices.end(), m_polygons[x].m_indices.begin(), m_polygons[x].m_indices.end());
	}
}

uint32_t Lime::Model::MeshData::GetIndexCount()
{
	if (!m_polygons.empty())
		return (uint32_t)(m_polygons[0].m_indices.size() * m_polygons.size());
	else
		return 0u;
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

void Lime::Model::Model3D::SetType(Model::MeshType type)
{
	m_modelType = type;
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
		MeshLoader::GetDefaulMeshInfo(id, *this);
		m_modelType = MESH;
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

MeshID Lime::Model::MeshLoader::LoadModel(const std::string& filename, ModelType type)
{
	MeshID result = -1;
	std::string ext;
	GetFileExt(filename, ext);
	//Proccess fbx files
	if (ext.compare("fbx") == 0)
	{
		FbxManager* fbxManager = NULL;
		FbxScene* scene = NULL;

		ENFORCE_SUCCESS(InitFBXObjects(fbxManager, scene), true, goto close);
		ENFORCE_SUCCESS(LoadFBXSceneFromFile(fbxManager, scene, filename), true, goto close);

		//Can have multilpe nodes although this program only loads the first node
		FbxNode* node = scene->GetRootNode()->GetChild(0);
		if (node)
		{
			std::shared_ptr<MeshData> mesh = nullptr;
			MeshDefaultSettings settings = {};

			//Populates structures above and saves the to a desired MeshLibrary.
			Create3DMeshFromFBX(node, mesh, settings, type);
			if(mesh != nullptr)
				result = SaveInformation(MESHLIB, mesh, settings);
		}

	close: 
		fbxManager->Destroy();

	}
	return result;
}

MeshID Lime::Model::MeshLoader::LoadModel(std::vector<Vertex> verts, std::vector<uint32_t> indices)
{
	std::shared_ptr<MeshData> mesh = nullptr;
	MeshDefaultSettings settings = {};

	return SaveInformation(MESHLIB, mesh, settings);
}

void Lime::Model::MeshLoader::GrabMeshData(MeshID id, std::shared_ptr<MeshData> & ptr)
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

void Lime::Model::MeshLoader::GetDefaulMeshInfo(MeshID id, Model3D & ptr)
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

MeshID Lime::Model::MeshLoader::SaveInformation(MeshLibrary & library, const std::shared_ptr<MeshData> & data, const MeshDefaultSettings & settings)
{
	return library.SaveMesh(data, settings);
}

bool Lime::Model::MeshLoader::InitFBXObjects(FbxManager *& manager, FbxScene *& scene)
{
	manager = FbxManager::Create();
	if (!manager)
		return false;

	//Gets the current version of the fbx sdk
	//fbxManager->GetVersion();
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	scene = FbxScene::Create(manager, "Imported scene");
	if (!scene)
		return false;

	return true;
}

bool Lime::Model::MeshLoader::LoadFBXSceneFromFile(FbxManager * manager, FbxScene * scene, const std::string & filename)
{
	//GetVersion for error checking
	//FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
	FbxImporter* fileReader = FbxImporter::Create(manager, "Fbx FileReader");
	const bool importStatus = fileReader->Initialize(filename.c_str(), -1, manager->GetIOSettings());
	if (!importStatus)
	{
		//Gets the error from load fail
		//FbxString error = fileReader->GetStatus().GetErrorString();
		//FBXSDK_printf("Call to FbxImporter::CompileVertexData() failed.\n");
		//FBXSDK_printf("Error returned: %s\n\n", error.BMBuffer());

		if (fileReader->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			//FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			//FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	//Check the Autodesk file type
	if (fileReader->IsFBX())
	{
		bool success = fileReader->Import(scene);
		if (!success)
		{
			//Access files that have passwords
			if (fileReader->GetStatus().GetCode() == FbxStatus::ePasswordError)
			{
				//char lPassword[1024];
				//do
				//{
				//	//Used to implement password
				//	FBXSDK_printf("Please enter password: ");

				//	lPassword[0] = '\0';

				//	FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
				//		scanf("%s", lPassword);
				//	FBXSDK_CRT_SECURE_NO_WARNING_END

				//		FbxString lString(lPassword);

				//	IOS_REF(manager).SetStringProp(IMP_FBX_PASSWORD, lString);
				//	IOS_REF(manager).SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

				//	success = fileReader->Import(scene);
				//} while (!success);
				return false;
			}
			else
				return false;
		}
	}
	fileReader->Destroy();
	return true;
}

void Lime::Model::MeshLoader::Create3DMeshFromFBX(FbxNode* pNode, std::shared_ptr<MeshData>& data, MeshDefaultSettings& settings, ModelType type)
{
	auto tempdata = std::make_shared<MeshData>();
	FbxMesh* mesh = pNode->GetMesh();
	ENFORCE_SUCCESS(mesh, true, return);

	//Populate MeshData with FBX data
	c_uint totalPolygonCount = mesh->GetPolygonCount();
	int totalIndexCount = 0;
	for (uint32_t polyIndex = 0; polyIndex < totalPolygonCount; polyIndex++)
	{
		Polygon<Vertex> poly;
		int polyVertexCount = mesh->GetPolygonSize(polyIndex);
		ENFORCE_SUCCESS(polyVertexCount, 3, return);
		for (int vertexIndex = 0; vertexIndex < polyVertexCount; vertexIndex++)
		{
			Vertex vert;
			int currentVertIndex = mesh->GetPolygonVertex(polyIndex, vertexIndex);
			auto& fbxVert = mesh->GetControlPointAt(currentVertIndex);
			vert.m_position = FbxVec4ToGlmVec3(fbxVert);
			ENFORCE_SUCCESS(GetFBXTextureCoordinates(mesh, vert, totalIndexCount), true, return);
			ENFORCE_SUCCESS(GetFBXMeshNormals(mesh, vert, totalIndexCount), true, return);

			poly.m_vertices.push_back(vert);
			totalIndexCount++;
		}
		poly.m_indices.push_back(totalIndexCount-3);
		poly.m_indices.push_back(totalIndexCount-1);
		poly.m_indices.push_back(totalIndexCount-2);
		tempdata->m_polygons.push_back(poly);
	}
	data = tempdata;

	//Get Default Settings
	settings.scale = FbxVec4ToGlmVec3(pNode->LclScaling.Get());
	settings.rotation = FbxVec4ToGlmVec3(pNode->LclRotation.Get());
	settings.rotation.x = -glm::radians(settings.rotation.x);
	settings.rotation.y = glm::radians(settings.rotation.y);
	settings.rotation.z = glm::radians(settings.rotation.z);
	settings.translation = FbxVec4ToGlmVec3(pNode->LclTranslation.Get());
}

bool Lime::Model::MeshLoader::GetFBXTextureCoordinates(FbxMesh * mesh, Vertex& vert, int totalIndexCount)
{
	//Implement logging for error cases
	bool result = false;
	FbxStringList uvSetNameList;
	mesh->GetUVSetNames(uvSetNameList);

	//Only grabbing the first uv set
	const char* uvSetName = uvSetNameList.GetStringAt(0);
	FbxGeometryElementUV* uvElement = mesh->GetElementUV(uvSetName);

	//Set the search mode to grab data fast as we are not going to modify it
	uvElement->SetMappingMode(FbxLayerElement::EMappingMode::eByPolygonVertex);
	uvElement->SetReferenceMode(FbxLayerElement::EReferenceMode::eDirect);

	// only support mapping mode eByPolygonVertex and eByControlPoint
	switch (uvElement->GetMappingMode())
	{
		//Implement for later use
	case FbxGeometryElement::eByControlPoint:
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (uvElement->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			int uvIndex = uvElement->GetIndexArray().GetAt(totalIndexCount);
			FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
			vert.m_uv.x = (float)uv[0];
			vert.m_uv.y = 1 - (float)uv[1];
			result = true;

		}
			break;
		}
		break;

	default:
		break;
	}

	return result;
}

bool Lime::Model::MeshLoader::GetFBXMeshNormals(FbxMesh* mesh, Vertex& vert, int totalIndexCount)
{
	//Implement logging for error cases
	bool result = false;

	//Grab the first normal set
	FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);

	//Set the search mode to grab data fast as we are not going to modify it
	vertexNormal->SetMappingMode(FbxLayerElement::EMappingMode::eByPolygonVertex);
	vertexNormal->SetReferenceMode(FbxLayerElement::EReferenceMode::eDirect);

	switch (vertexNormal->GetMappingMode())
	{
		//Implement for later use
	case FbxGeometryElement::eByControlPoint:
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			int uvIndex = vertexNormal->GetIndexArray().GetAt(totalIndexCount);
			FbxVector4 normal = vertexNormal->GetDirectArray().GetAt(totalIndexCount);
			vert.m_normal = FbxVec4ToGlmVec3(normal);
			//Invert the output to work with DirectX coordinate system
			vert.m_normal = -vert.m_normal;
			result = true;
		}
		break;
		}
		break;
	}

	return result;
}

glm::vec3 Lime::Model::MeshLoader::FbxVec4ToGlmVec3(const FbxVector4& in)
{
	glm::vec3 output;
	output.x = (float)in.mData[0];
	output.y = (float)in.mData[1];
	output.z = (float)in.mData[2];
	return output;
}

MeshID Lime::Model::MeshLibrary::SaveMesh(const std::shared_ptr<MeshData>& mesh, const MeshDefaultSettings & setting)
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
