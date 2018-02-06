#include "FBXReader.h"
#include <iostream>

using namespace std;
using namespace SEF;

bool FBXReader::ReadFile(const std::string& file, std::vector<MeshData>& meshArr, std::vector<Skeleton>& skeletonArr, std::string& error, bool attatchment, int startBoneIndexCount)
{
	m_bAttatchment = attatchment;
	m_boneIndexOffset = startBoneIndexCount;
	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	if (!lImporter->Initialize(file.c_str(), -1, lSdkManager->GetIOSettings()))
	{
		error = std::string(lImporter->GetStatus().GetErrorString());
		return false;
	}
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);
	m_FBXScene = lScene;
	// The file is imported, so get rid of the importer.
	lImporter->Destroy();
	FbxAxisSystem::DirectX.ConvertScene(lScene);
	FbxNode* lRootNode = lScene->GetRootNode();
	fbxsdk::FbxNode * meshNode = nullptr;
	std::vector<MeshTempData> tempArr;
	if (lRootNode) {
		int childCount = lRootNode->GetChildCount();

		for (int i = 0; i < childCount; i++)
		{
			GetChildInfo(lRootNode->GetChild(i), meshNode, tempArr, skeletonArr, true);
		}
	}

	//Remove ':' char from name and replace with '_'
	for (auto& skel : skeletonArr)
	{
		for (auto& joints : skel.Joints)
		{
			for (auto& c : joints.Name)
			{
				if (c == ':')
					c = '_';
			}
		}
	}

	//Convert Temp data to MeshData
	for (const auto& temp : tempArr)
	{
		meshArr.push_back({});
		ExtractMeshDataFromTemp(temp, meshArr.back());
	}

	m_FBXScene = nullptr;
	lSdkManager->Destroy();
	return (!meshArr.empty() || !skeletonArr.empty());
}

void FBXReader::GetChildInfo(fbxsdk::FbxNode * pNode, fbxsdk::FbxNode*& meshNode, std::vector<MeshTempData>& meshArr, std::vector<Skeleton>& skeletonArr, bool parent)const
{
	int attribCount = pNode->GetNodeAttributeCount();
	for (int i = 0; i < attribCount; i++)
	{
		auto attrib = pNode->GetNodeAttributeByIndex(i);
		auto type = attrib->GetAttributeType();

		switch (type)
		{
		case FbxNodeAttribute::eMesh:
		{
			MeshTempData mesh = {};
			ReadVertexInfo(pNode, mesh);
			if (mesh.Size != 0)
			{
				meshNode = pNode;
				meshArr.push_back(mesh);
			}
		}
		break;

		case FbxNodeAttribute::eSkeleton:
		{
			Skeleton skeleton = {};
			GetSkeleton(pNode, skeleton.Joints);
			if (!skeleton.Joints.empty() && meshNode)
			{
				skeleton.SkinName = meshNode->GetName();
				auto* currMesh = FindMeshByName(skeleton.SkinName, meshArr);
				GetSkinWeightData(meshNode, currMesh, skeleton);
				skeletonArr.push_back(skeleton);
			}
		}
		return;

		}
	}

	int childCount = pNode->GetChildCount();
	for (int j = 0; j < childCount; j++)
	{
		auto* childNode = pNode->GetChild(j);
		GetChildInfo(childNode, meshNode, meshArr, skeletonArr);
	}
}

uint32_t FBXReader::GetSkeleton(fbxsdk::FbxNode* pNode, std::vector<SEF::Joint>& joints)const
{
	Joint current = {};
	uint32_t currentIndex = (uint32_t)joints.size();

	current.Name = pNode->GetName();
	joints.push_back(current);

	for (int index = 0; index < pNode->GetChildCount(); ++index)
	{
		uint32_t childIndex = GetSkeleton(pNode->GetChild(index), joints);
		joints[currentIndex].Children.push_back(childIndex);
	}

	return currentIndex;
}

void FBXReader::ReadVertexInfo(fbxsdk::FbxNode * pNode, MeshTempData& mesh)const
{
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	auto fbxMesh = pNode->GetMesh();
	if (fbxMesh)
	{
		mesh.Name = pNode->GetName();
		if (fbxMesh->IsTriangleMesh())
		{
			int vertexCount = 0;
			fbxMesh->GenerateTangentsDataForAllUVSets(true);
			for (int pIndex = 0; pIndex < fbxMesh->GetPolygonCount(); pIndex++)
			{
				for (int vIndex = 0; vIndex < 3; vIndex++)
				{
					VertexData vertex = {};
					//Control Points represent only the positional aspect ov a Vertex
					//In a cube, a Control Point can represent 3 Vertices (a vertex for each face)
					int ctrlPointIndex = fbxMesh->GetPolygonVertex(pIndex, vIndex);
					auto pos = fbxMesh->GetControlPointAt(ctrlPointIndex);
					vertex.Position.x = static_cast<float>(pos.mData[0]);
					vertex.Position.y = static_cast<float>(pos.mData[1]);
					vertex.Position.z = static_cast<float>(pos.mData[2]);

					ReadVertexInfo(fbxMesh, ctrlPointIndex, vertexCount, vertex);
					AddNewVertex(mesh, vertex, ctrlPointIndex);
					vertexCount++;
				}
			}
		}
	}
}

bool FBXReader::GetSkinWeightData(fbxsdk::FbxNode * pNode, MeshTempData* mesh, Skeleton& skeleton)const
{
	if (!mesh)
		return false;

	FbxMesh* currMesh = pNode->GetMesh();
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	// This geometry transform is something I cannot understand
	// I think it is from MotionBuilder
	// If you are using Maya for your models, 99% this is just an
	// identity matrix
	// But I am taking it into account anyways......
	FbxAMatrix geometryTransform = FBXReader::GetGeometryTransformation(pNode);

	// A deformer is a FBX thing, which contains some clusters
	// A cluster contains a link, which is basically a joint
	// Normally, there is only one deformer in a mesh
	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		// There are many types of deformers in Maya,
		// We are using only skins, so we see if this is a skin
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin)
		{
			continue;
		}

		unsigned int numOfClusters = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			unsigned int currJointIndex = FindJointIndexUsingName(skeleton, currJointName);
			if (currJointIndex == -1)
			{
				throw std::exception();
				return false;
			}

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			// Update the information in mSkeleton 
			skeleton.Joints[currJointIndex].GlobalBindPoseInverse = FBXToMatrix4f(globalBindposeInverseMatrix);

			// Associate each joint with the control points it affects
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				JointData blendFactor = {};
				blendFactor.ID = currJointIndex;
				blendFactor.Weight = (float)currCluster->GetControlPointWeights()[i];
				int FBXIndex = currCluster->GetControlPointIndices()[i];

				//Increment index if this is addon
				if (m_bAttatchment)
				{
					blendFactor.ID += m_boneIndexOffset;
				}

				//Add the blend factor to each vertex located at the given position index (FBXIndex)
				auto fbxIndexList = mesh->Library.find(FBXIndex);
				if (fbxIndexList != mesh->Library.end())
				{
					for (auto& vertex : fbxIndexList->second)
					{
						AddBlendInfo(vertex.second, blendFactor);
					}
				}
			}
		}
	}
	return true;
}

int FBXReader::FindJointIndexUsingName(Skeleton & skeleton, const std::string & name) const
{
	for (int index = 0; index < skeleton.Joints.size(); index++)
	{
		if (!name.compare(skeleton.Joints[index].Name))
		{
			return index;
		}
	}
	return -1;
}

FbxAMatrix FBXReader::GetGeometryTransformation(FbxNode* inNode)const
{
	if (!inNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

void FBXReader::AddNewVertex(MeshTempData& data, const VertexData& vertex, int FBXIndex) const
{
	//FBXIndex refers to the position aspect of the vertex only!

	int ID = -1;

	//If the index has been recorded else create and add new 
	auto fbxIndexList = data.Library.find(FBXIndex);
	if (fbxIndexList != data.Library.end())
	{
		//Check each vertex element to see if it has been added
		//Record ID if found
		for (const auto& v : fbxIndexList->second)
		{
			if (IsEqual(v.second, vertex))
			{
				ID = v.first;
				data.Index.push_back(ID);
				break;
			}
		}
		//If not found, create and add the new vertex
		if (ID == -1)
		{
			ID = data.Size++;
			auto entry = std::make_pair(ID, vertex);
			fbxIndexList->second.push_back(entry);
			data.Index.push_back(ID);
		}
	}
	else
	{
		ID = data.Size++;
		auto entry = std::make_pair(ID, vertex);
		data.Library.emplace(FBXIndex, std::vector<std::pair<int, VertexData>>());
		data.Library[FBXIndex].push_back(entry);
		data.Index.push_back(ID);
	}
}

void FBXReader::ReadVertexInfo(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, VertexData& outVertex)const
{
	if (inMesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);
	FbxGeometryElementVertexColor* vertexColor = inMesh->GetElementVertexColor(0);
	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(0);
	FbxGeometryElementTangent* vertexTangent = inMesh->GetElementTangent(0);
	FbxGeometryElementBinormal* vertexBinormal = inMesh->GetElementBinormal(0);

	auto& outNormal = outVertex.Normal;
	auto& outColor = outVertex.Color = InitVector3(1.0f, 1.0f, 1.0f);
	auto& outUV = outVertex.UV;
	auto& outTangent = outVertex.Tangent;
	auto& outBinormal = outVertex.Binormal;
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

			if (vertexColor)
			{
				outColor.x = static_cast<float>(vertexColor->GetDirectArray().GetAt(inCtrlPointIndex).mRed);
				outColor.y = static_cast<float>(vertexColor->GetDirectArray().GetAt(inCtrlPointIndex).mGreen);
				outColor.z = static_cast<float>(vertexColor->GetDirectArray().GetAt(inCtrlPointIndex).mBlue);
			}

			if (vertexUV)
			{
				outVertex.bHasUV = true;
				outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
				outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			}

			if (vertexTangent)
			{
				outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
				outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
				outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
			}

			if (vertexBinormal)
			{
				outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
				outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
				outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
			}
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);

			if (vertexColor)
			{
				outColor.x = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mRed);
				outColor.y = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mGreen);
				outColor.z = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mBlue);
			}

			if (vertexUV)
			{
				outVertex.bHasUV = true;
				outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
				outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
			}

			if (vertexTangent)
			{
				outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			}

			if (vertexBinormal)
			{
				outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
				outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
				outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			}
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

			if (vertexColor)
			{
				outColor.x = static_cast<float>(vertexColor->GetDirectArray().GetAt(inVertexCounter).mRed);
				outColor.y = static_cast<float>(vertexColor->GetDirectArray().GetAt(inVertexCounter).mGreen);
				outColor.z = static_cast<float>(vertexColor->GetDirectArray().GetAt(inVertexCounter).mBlue);
			}

			if (vertexUV)
			{
				outVertex.bHasUV = true;
				outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inVertexCounter).mData[0]);
				outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			}

			if (vertexTangent)
			{
				outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[0]);
				outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[1]);
				outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[2]);
			}

			if (vertexBinormal)
			{
				outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
				outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
				outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
			}
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);

			if (vertexColor)
			{
				outColor.x = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mRed);
				outColor.y = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mGreen);
				outColor.z = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mBlue);
			}

			if (vertexUV)
			{
				outVertex.bHasUV = true;
				outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
				outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
			}

			if (vertexTangent)
			{
				outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			}

			if (vertexBinormal)
			{
				outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
				outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
				outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			}
		}
		break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

inline void ExtractMeshDataFromTemp(const MeshTempData & temp, MeshData & data)
{
	data.Name = temp.Name;
	data.Indices = temp.Index;
	data.Vertices.resize(temp.Size);
	for (const auto& posList : temp.Library)
	{
		for (const auto& vertex : posList.second)
		{
			data.Vertices[vertex.first] = vertex.second;
		}
	}
}

inline MeshTempData* FindMeshByName(const std::string & name, std::vector<MeshTempData>& meshArr)
{
	for (auto it = meshArr.begin(); it != meshArr.end(); ++it)
	{
		if (it->Name == name)
		{
			return &(*it);
		}
	}
	return nullptr;
}

Matrix4f FBXToMatrix4f(const fbxsdk::FbxAMatrix & matrix)
{
	Matrix4f retVal = {};
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			retVal.Properties[x][y] = (float)matrix.Get(x, y);
		}
	}
	return retVal;
}
