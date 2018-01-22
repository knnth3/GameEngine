#include "FBX_READER.h"
#include <iostream>

using namespace std;

bool FBX_READER::LoadModel(const std::string& file, MeshData& mesh, const std::string& ext, std::string& error)const
{
	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	//Try to load real file 
	if (!lImporter->Initialize(file.c_str(), -1, lSdkManager->GetIOSettings()))
	{
		error = std::string(lImporter->GetStatus().GetErrorString());
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
		{
			Skeleton skeleton;
			GetMesh(lRootNode->GetChild(i), meshes);
			GetSkeleton(lRootNode->GetChild(i), skeleton);
		}
	}

	lSdkManager->Destroy();
	if (meshes.empty())
		return false;

	mesh = meshes[0];
	return true;
}

bool FBX_READER::SaveNewMesh(const std::string& file, MeshData mesh)const
{
	if (!file.empty())
	{
		int indexCount = (int)mesh.m_indices.size();
		int vertexCount = (int)mesh.m_vertices.size();

		// Create the buffer
		int offset = 0;
		int arrSize = (sizeof(int) * 2) + (sizeof(unsigned int) * indexCount) + (sizeof(VertexData) * vertexCount);
		std::vector<unsigned char> arr;

		//Wite to temp buffer
		arr.insert(arr.end(), (char*)&indexCount, (char*)&indexCount + sizeof(int));
		arr.insert(arr.end(), (char*)&vertexCount, (char*)&vertexCount + sizeof(int));

		for (int i = 0; i < indexCount; i++)
			arr.insert(arr.end(), (char*)&mesh.m_indices[i], (char*)&mesh.m_indices[i] + sizeof(unsigned int));

		for (int i = 0; i < vertexCount; i++)
			arr.insert(arr.end(), (char*)&mesh.m_vertices[i], (char*)&mesh.m_vertices[i] + sizeof(VertexData));

		// Write bytes to a file using a buffer
		return true;
	}
	return false;
}

void FBX_READER::GetMesh(fbxsdk::FbxNode * pNode, std::vector<MeshData>& meshes)const
{
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	// Read the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	{
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
					meshData.m_vertices.resize(mesh->GetControlPointsCount());
					meshData.m_indexedIDs.resize(mesh->GetControlPointsCount(), false);
					int vertexCount = 0;
					for (int pIndex = 0; pIndex < mesh->GetPolygonCount(); pIndex++)
					{
						for (int vIndex = 0; vIndex < 3; vIndex++)
						{
							VertexData vertex;
							int ctrlPointIndex = mesh->GetPolygonVertex(pIndex, vIndex);
							auto pos = mesh->GetControlPointAt(ctrlPointIndex);
							vertex.m_position.x = static_cast<float>(pos.mData[0]);
							vertex.m_position.y = static_cast<float>(pos.mData[1]);
							vertex.m_position.z = static_cast<float>(pos.mData[2]);

							ReadNormal(mesh, ctrlPointIndex, vertexCount, vertex.m_normal);
							AddNewVertex(meshData, vertex, ctrlPointIndex);
							vertexCount++;
						}
					}
					meshes.push_back(meshData);
				}
			}
		}
	}

	// Recursively read the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
		GetMesh(pNode->GetChild(j), meshes);

	printf("\n");
}

void FBX_READER::GetVertexSkeletonInfo(fbxsdk::FbxNode * pNode, std::vector<MeshData>& meshes, Skeleton& skeleton)
{
	FbxMesh* currMesh = pNode->GetMesh();
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	// This geometry transform is something I cannot understand
	// I think it is from MotionBuilder
	// If you are using Maya for your models, 99% this is just an
	// identity matrix
	// But I am taking it into account anyways......
	FbxAMatrix geometryTransform = FBX_READER::GetGeometryTransformation(pNode);

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
				return;
			}

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			// Update the information in mSkeleton 
			//skeleton.Joints[currJointIndex].GlobalBindPoseInverse = globalBindposeInverseMatrix;

			// Associate each joint with the control points it affects
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				JointData blendFactor;
				blendFactor.ID = currJointIndex;
				blendFactor.Weight = (float)currCluster->GetControlPointWeights()[i];
				int vertexIndex = currCluster->GetControlPointIndices()[i];
				meshes[0].m_vertices[vertexIndex].AddBlendInfo(blendFactor);
			}

			// Get animation information
			// Now only supports one take
			//FbxAnimStack* currAnimStack = mFBXScene->GetSrcObject(0);
			//FbxString animStackName = currAnimStack->GetName();
			//mAnimationName = animStackName.Buffer();
			//FbxTakeInfo* takeInfo = mFBXScene->GetTakeInfo(animStackName);
			//FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			//FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			//mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
			//Keyframe** currAnim = &skeleton.mJoints[currJointIndex].mAnimation;

			//for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
			//{
			//	FbxTime currTime;
			//	currTime.SetFrame(i, FbxTime::eFrames24);
			//	*currAnim = new Keyframe();
			//	(*currAnim)->mFrameNum = i;
			//	FbxAMatrix currentTransformOffset = pNode->EvaluateGlobalTransform(currTime) * geometryTransform;
			//	(*currAnim)->mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);
			//	currAnim = &((*currAnim)->mNext);
			//}
		}
	}
}

void FBX_READER::GetSkeleton(fbxsdk::FbxNode* pNode, Skeleton& skeleton)const
{
	auto childCount = pNode->GetChildCount();
	if (pNode->GetNodeAttribute() && pNode->GetNodeAttribute()->GetAttributeType() && pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint child;
		child.Name = pNode->GetName();
		skeleton.Joints.push_back(child);
	}
	for (int childIndex = 0; childIndex < childCount; ++childIndex)
	{
		GetSkeleton(pNode->GetChild(childIndex), skeleton);
	}
}

int FBX_READER::FindJointIndexUsingName(Skeleton & skeleton, const std::string & name) const
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

FbxAMatrix FBX_READER::GetGeometryTransformation(FbxNode* inNode)const
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

void FBX_READER::AddNewVertex(MeshData& data, VertexData vertex, int FBXIndex) const
{
	//Assumes the vertex array and the ID array have been
	//resized to fit the incoming vertices
	bool newVert = true;
	if (data.m_vertices.size() > FBXIndex)
	{
		if (data.m_indexedIDs[FBXIndex])
		{
			newVert = false;
			data.m_indices.push_back(FBXIndex);
		}
	}
	if (newVert)
	{
		int newIndex = (int)data.m_vertices.size();
		data.m_indices.push_back(newIndex);
		data.m_indexedIDs[FBXIndex] = true;
		data.m_vertices[FBXIndex] = vertex;
	}
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