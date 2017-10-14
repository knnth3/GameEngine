#include "ModelLoader.h"

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

MeshID Lime::Model::MeshLoader::LoadModel(const std::string& filename, MeshType type)
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
			if (mesh != nullptr)
				result = SaveInformation(MESHLIB, mesh, settings);
		}

	close:
		fbxManager->Destroy();

	}
	return result;
}

MeshID Lime::Model::MeshLoader::LoadModel(const std::vector<Vertex>& verts, const std::vector<uint32_t>& indices)
{
	std::shared_ptr<MeshData> mesh = std::make_shared<MeshData>();
	MeshDefaultSettings settings = {};
	settings.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	int counter = 0;
	for (int x = 0; x < (int)(verts.size() / 3); x++)
	{
		Polygon<Vertex> p;
		p.m_vertices.insert(p.m_vertices.end(), verts.begin() + counter, verts.begin() + counter + 3);
		p.m_indices.insert(p.m_indices.end(), indices.begin() + counter, indices.begin() + counter + 3);

		mesh->m_polygons.push_back(p);
		counter += 3;
	}


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

void Lime::Model::MeshLoader::Create3DMeshFromFBX(FbxNode* pNode, std::shared_ptr<MeshData>& data, MeshDefaultSettings& settings, MeshType type)
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
		poly.m_indices.push_back(totalIndexCount - 3);
		poly.m_indices.push_back(totalIndexCount - 1);
		poly.m_indices.push_back(totalIndexCount - 2);
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
