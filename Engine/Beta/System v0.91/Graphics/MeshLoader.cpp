#include "MeshLoader.h"

//-Checks a functions return value agains desired result
//-does whatever instructed to do on fail;
#define ENFORCE_SUCCESS(function, desiredResult, whatToDoOnFail) \
		if (!desiredResult == function) { whatToDoOnFail; }

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF(fbxManager) (*(fbxManager->GetIOSettings()))
#endif

using namespace Graphics;
using namespace std;

static MeshLibrary MESHLIB;

Graphics::MeshID Graphics::MeshLoader::LoadModel(const std::string& filename)
{
	MeshID result = MESHLIB.IsFilepathQuerried(filename);
	if (result == -1)
	{
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
				std::shared_ptr<Mesh> mesh = nullptr;
				MeshDefaultSettings settings = {};

				//Populates structures above and saves the to a desired MeshLibrary.
				Create3DMeshFromFBX(node, mesh, settings);
				if (mesh != nullptr)
				{
					result = SaveInformation(MESHLIB, mesh, settings);
					MESHLIB.m_filepaths.emplace(filename, result);
				}
			}

		close:
			fbxManager->Destroy();

		}
	}

	return result;
}

Graphics::MeshID Graphics::MeshLoader::LoadModel(const std::vector<Vertex>& verts, const std::vector<Index>& indices, const std::string uniqueName)
{
	MeshID result = MESHLIB.IsKeyNameQuerried(uniqueName);
	if(result == -1)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		MeshDefaultSettings settings = {};
		settings.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		Polygon p;
		p.m_vertices.insert(p.m_vertices.end(), verts.begin(), verts.end());
		p.m_indices.insert(p.m_indices.end(), indices.begin(), indices.end());
		mesh->m_polygons.push_back(p);

		result = SaveInformation(MESHLIB, mesh, settings);
		MESHLIB.m_keyNames.emplace(uniqueName, result);
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

	return SaveInformation(MESHLIB, mesh, settings);
}

void Graphics::MeshLoader::Clear()
{
	MESHLIB.Clear();
}

void Graphics::MeshLoader::GrabMeshData(MeshID id, std::shared_ptr<Mesh> & ptr)
{
	if (MESHLIB.m_modelLibrary.size() > id && id >= 0)
		ptr = MESHLIB.m_modelLibrary.at(id);
	else
		ptr = MESHLIB.m_default;
}

void Graphics::MeshLoader::GetDefaulMeshInfo(MeshID id, MeshDefaultSettings& settings)
{
	if(MESHLIB.m_defaultSettings.size() > id)
		settings = MESHLIB.m_defaultSettings.at(id);
}

Graphics::MeshID Graphics::MeshLoader::SaveInformation(MeshLibrary & library, const std::shared_ptr<Mesh> & data, const MeshDefaultSettings & settings)
{
	return library.SaveMesh(data, settings);
}

bool Graphics::MeshLoader::InitFBXObjects(FbxManager *& manager, FbxScene *& scene)
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

bool Graphics::MeshLoader::LoadFBXSceneFromFile(FbxManager * manager, FbxScene * scene, const std::string & filename)
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

void Graphics::MeshLoader::Create3DMeshFromFBX(FbxNode* pNode, std::shared_ptr<Mesh>& data, MeshDefaultSettings& settings)
{
	auto tempdata = std::make_shared<Mesh>();
	FbxMesh* mesh = pNode->GetMesh();
	ENFORCE_SUCCESS(mesh, true, return);

	//Populate Mesh with FBX data
	float height = 0.0f;
	const unsigned int totalPolygonCount = mesh->GetPolygonCount();
	int totalIndexCount = 0;
	for (uint32_t polyIndex = 0; polyIndex < totalPolygonCount; polyIndex++)
	{
		Polygon poly;
		int polyVertexCount = mesh->GetPolygonSize(polyIndex);
		ENFORCE_SUCCESS(polyVertexCount, 3, return);
		for (int vertexIndex = 0; vertexIndex < polyVertexCount; vertexIndex++)
		{
			Vertex vert;
			int currentVertIndex = mesh->GetPolygonVertex(polyIndex, vertexIndex);
			auto& fbxVert = mesh->GetControlPointAt(currentVertIndex);
			vert.m_position = FbxVec4ToGlmVec3(fbxVert);
			if (vert.m_position.y > height)
				height = vert.m_position.y;

			ENFORCE_SUCCESS(GetFBXTextureCoordinates(mesh, vert, totalIndexCount), true, return);
			ENFORCE_SUCCESS(GetFBXMeshNormals(mesh, vert, totalIndexCount), true, return);

			poly.m_vertices.push_back(vert);
			totalIndexCount++;
		}
		poly.m_indices.push_back(totalIndexCount - 3);
		poly.m_indices.push_back(totalIndexCount - 1);
		poly.m_indices.push_back(totalIndexCount - 2);
		tempdata->m_polygons.push_back(poly);
	}
	data = tempdata;
	data->m_height = height;

	//Get Default Settings
	settings.scale = FbxVec4ToGlmVec3(pNode->LclScaling.Get());
	settings.rotation = FbxVec4ToGlmVec3(pNode->LclRotation.Get());
	settings.rotation.x = -glm::radians(settings.rotation.x);
	settings.rotation.y = glm::radians(settings.rotation.y);
	settings.rotation.z = glm::radians(settings.rotation.z);
	settings.translation = FbxVec4ToGlmVec3(pNode->LclTranslation.Get());
}

bool Graphics::MeshLoader::GetFBXTextureCoordinates(FbxMesh * mesh, Vertex& vert, int totalIndexCount)
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

bool Graphics::MeshLoader::GetFBXMeshNormals(FbxMesh* mesh, Vertex& vert, int totalIndexCount)
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

glm::vec3 Graphics::MeshLoader::FbxVec4ToGlmVec3(const FbxVector4& in)
{
	glm::vec3 output;
	output.x = (float)in.mData[0];
	output.y = (float)in.mData[1];
	output.z = (float)in.mData[2];
	return output;
}

Graphics::MeshLibrary::MeshLibrary()
{
	MeshID result = MeshLoader::LoadModel("Assets/Models/Cube_TextureWrap.fbx");
	if (result != -1)
	{
		m_default = m_modelLibrary[result];
		Clear();
	}
}

Graphics::MeshID Graphics::MeshLibrary::SaveMesh(const std::shared_ptr<Mesh>& mesh, const MeshDefaultSettings & setting)
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

void Graphics::MeshLibrary::Clear()
{
	m_modelLibrary.clear();
	m_defaultSettings.clear();
	m_filepaths.clear();
	m_keyNames.clear();
}

Graphics::MeshID Graphics::MeshLibrary::IsFilepathQuerried(const std::string filepath)
{
	MeshID ID = -1;
	if (!filepath.empty())
	{
		auto result = m_filepaths.find(filepath);
		if (result != m_filepaths.end())
			ID = result->second;
	}

	return ID;
}

Graphics::MeshID Graphics::MeshLibrary::IsKeyNameQuerried(const std::string filepath)
{
	MeshID ID = -1;
	if (!filepath.empty())
	{
		auto result = m_keyNames.find(filepath);
		if (result != m_keyNames.end())
			ID = result->second;
	}

	return ID;
}
