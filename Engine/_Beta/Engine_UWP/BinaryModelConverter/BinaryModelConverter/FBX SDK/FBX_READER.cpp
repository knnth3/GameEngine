#include "pch.h"
#include "FBX_READER.h"
#include <iostream>

using namespace BinaryModelConverter;
using namespace std;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;


bool FBX_READER::LoadModel(Windows::Storage::StorageFile^ file, MeshData& mesh, Platform::String^ ext, Platform::String^& error)const
{
	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	//Get the filetype before loading real file
	std::string filetype = "model" + to_str(ext->Data());
	lImporter->Initialize(filetype.c_str(), -1, lSdkManager->GetIOSettings());
	int format = lImporter->GetFileFormat();

	//Try to load real file 
	if (!lImporter->Initialize(to_str(file->Path->Data()).c_str(), format, lSdkManager->GetIOSettings()))
	{
		auto e = std::string(lImporter->GetStatus().GetErrorString());
		error = ref new Platform::String(to_wstr(e).c_str());
		return false;
	}
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();
	std::vector<MeshData> meshes;
	FbxAxisSystem::DirectX.ConvertScene(lScene);
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i), meshes);
	}

	lSdkManager->Destroy();
	if (meshes.empty())
		return false;

	mesh = meshes[0];
	return true;
}

bool FBX_READER::SaveNewMesh(Windows::Storage::StorageFile^ file, MeshData mesh)const
{
	if (file)
	{
		// Create the buffer
		int offset = 0;
		int arrSize = (sizeof(int) * 2) + (sizeof(unsigned int) * mesh.m_indexCount) + (sizeof(VertexData) * mesh.m_vertexCount);
		std::vector<unsigned char> arr;

		//Wite to temp buffer
		arr.insert(arr.end(), (char*)&mesh.m_indexCount, (char*)&mesh.m_indexCount + sizeof(int));
		arr.insert(arr.end(), (char*)&mesh.m_vertexCount, (char*)&mesh.m_vertexCount + sizeof(int));

		for (int i = 0; i < mesh.m_indexCount; i++)
			arr.insert(arr.end(), (char*)&mesh.m_indices[i], (char*)&mesh.m_indices[i] + sizeof(unsigned int));

		for (int i = 0; i < mesh.m_vertexCount; i++)
			arr.insert(arr.end(), (char*)&mesh.m_vertices[i], (char*)&mesh.m_vertices[i] + sizeof(VertexData));

		// Write bytes to a file using a buffer
		Array<unsigned char>^ data = ref new Array<unsigned char>(arr.data(), (int)arr.size());
		create_task(FileIO::WriteBytesAsync(file, data));
		return true;
	}
	return false;
}

void FBX_READER::PrintNode(fbxsdk::FbxNode * pNode, std::vector<MeshData>& meshes)const
{
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	// Print the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	{
		//PrintAttribute(pNode->GetNodeAttributeByIndex(i));
		auto attrib = pNode->GetNodeAttributeByIndex(i);
		auto name = attrib->GetName();
		auto type = attrib->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			auto mesh = pNode->GetMesh();
			if (mesh)
			{
				if (mesh->IsTriangleMesh())
				{
					MeshData meshData;
					int vertexCount = 0;
					for (int x = 0; x < mesh->GetPolygonCount(); x++)
					{
						for (int y = 0; y < 3; y++)
						{
							VertexData vertex;
							int ctrlPointIndex = mesh->GetPolygonVertex(x, y);
							auto pos = mesh->GetControlPointAt(ctrlPointIndex);
							vertex.m_position.x = static_cast<float>(pos.mData[0]);
							vertex.m_position.y = static_cast<float>(pos.mData[1]);
							vertex.m_position.z = static_cast<float>(pos.mData[2]);

							ReadNormal(mesh, ctrlPointIndex, vertexCount, vertex.m_normal);
							AddNewVertex(meshData, vertex);
							vertexCount++;
						}
					}

					meshes.push_back(meshData);
				}
			}
		}
	}

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
		PrintNode(pNode->GetChild(j), meshes);

	printf("\n");
}

std::string FBX_READER::to_str(std::wstring original)const
{
	std::string convert;
	convert.insert(convert.begin(), original.begin(), original.end());
	return convert;
}

std::wstring FBX_READER::to_wstr(std::string original)const
{
	std::wstring convert;
	convert.insert(convert.begin(), original.begin(), original.end());
	return convert;
}

void FBX_READER::AddNewVertex(MeshData& data, VertexData vertex) const
{
	bool newVert = true;
	unsigned int index = 0;
	unsigned int polledIndex = 0;
	for (auto verts : data.m_vertices)
	{
		if (verts == vertex)
		{
			newVert = false;
			polledIndex = index;
		}
		index++;
	}
	if (newVert)
	{
		data.m_indices.push_back(index);
		data.m_vertices.push_back(vertex);
		data.m_vertexCount++;
	}
	else
		data.m_indices.push_back(polledIndex);

	data.m_indexCount++;
}

void FBX_READER::ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, Vector3<float>& outNormal)const
{
	if (inMesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}
	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}