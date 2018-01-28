#include "FBXReader.h"
#include <iostream>

using namespace std;

bool FBXReader::ReadFile(const std::string& file, std::vector<MeshData>& meshArr, std::vector<Skeleton>& skeletonArr, std::string& error)const
{
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

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();
	FbxAxisSystem::DirectX.ConvertScene(lScene);
	FbxNode* lRootNode = lScene->GetRootNode();
	fbxsdk::FbxNode * meshNode = nullptr;

	if (lRootNode) {
		int childCount = lRootNode->GetChildCount();
		for (int i = 0; i < childCount; i++)
		{
			GetChildInfo(lRootNode->GetChild(i), meshNode, meshArr, skeletonArr, true);
		}
	}

	lSdkManager->Destroy();
	return (!meshArr.empty() || !skeletonArr.empty());
}

void FBXReader::GetChildInfo(fbxsdk::FbxNode * pNode, fbxsdk::FbxNode*& meshNode, std::vector<MeshData>& meshArr, std::vector<Skeleton>& skeletonArr, bool parent)const
{
	Skeleton skeleton;
	MeshData mesh;

	int attribCount = pNode->GetNodeAttributeCount();
	for (int i = 0; i < attribCount; i++)
	{
		auto attrib = pNode->GetNodeAttributeByIndex(i);
		auto type = attrib->GetAttributeType();

		switch (type)
		{
		case FbxNodeAttribute::eMesh:
			GetMesh(pNode, mesh);
			if (!mesh.Vertices.empty())
			{
				meshNode = pNode;
				meshArr.push_back(mesh);
			}
			break;

		case FbxNodeAttribute::eSkeleton:
			if (meshNode)
			{
				skeleton.SkinName = meshNode->GetName();
				GetSkeleton(pNode, skeleton);
				if (!skeleton.Joints.empty())
				{
					auto& currMesh = meshArr.back();
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

void FBXReader::GetSkeleton(fbxsdk::FbxNode* pNode, Skeleton& skeleton)const
{
	Joint child;
	child.Name = pNode->GetName();
	skeleton.Joints.push_back(child);

	auto childCount = pNode->GetChildCount();
	for (int childIndex = 0; childIndex < childCount; ++childIndex)
	{
		GetSkeleton(pNode->GetChild(childIndex), skeleton);
	}
}

void FBXReader::GetMesh(fbxsdk::FbxNode * pNode, MeshData& mesh)const
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
			mesh.Vertices.resize(fbxMesh->GetControlPointsCount());
			mesh.IndexedIDs.resize(fbxMesh->GetControlPointsCount(), false);
			int vertexCount = 0;
			for (int pIndex = 0; pIndex < fbxMesh->GetPolygonCount(); pIndex++)
			{
				for (int vIndex = 0; vIndex < 3; vIndex++)
				{
					VertexData vertex;
					int ctrlPointIndex = fbxMesh->GetPolygonVertex(pIndex, vIndex);
					auto pos = fbxMesh->GetControlPointAt(ctrlPointIndex);
					vertex.Position.x = static_cast<float>(pos.mData[0]);
					vertex.Position.y = static_cast<float>(pos.mData[1]);
					vertex.Position.z = static_cast<float>(pos.mData[2]);

					ReadNormal(fbxMesh, ctrlPointIndex, vertexCount, vertex.Normal);
					AddNewVertex(mesh, vertex, ctrlPointIndex);
					vertexCount++;
				}
			}
		}
	}
}

void FBXReader::GetSkinWeightData(fbxsdk::FbxNode * pNode, MeshData& mesh, Skeleton& skeleton)const
{
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
				return;
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
				int vertexIndex = currCluster->GetControlPointIndices()[i];
				mesh.Vertices[vertexIndex].AddBlendInfo(blendFactor);
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

void FBXReader::AddNewVertex(MeshData& data, VertexData vertex, int FBXIndex) const
{
	//Assumes the vertex array and the ID array have been
	//resized to fit the incoming vertices

	bool newVert = true;
	if (data.IndexedIDs[FBXIndex])
	{
		newVert = false;
		data.Indices.push_back(FBXIndex);
	}
	if (newVert)
	{
		data.Indices.push_back(FBXIndex);
		data.IndexedIDs[FBXIndex] = true;
		data.Vertices[FBXIndex] = vertex;
	}
}

void FBXReader::ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, Vector3<float>& outNormal)const
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

Matrix4f FBXToMatrix4f(const fbxsdk::FbxAMatrix & matrix)
{
	Matrix4f retVal;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			retVal.Properties[x][y] = (float)matrix.Get(x, y);
		}
	}
	return retVal;
}
