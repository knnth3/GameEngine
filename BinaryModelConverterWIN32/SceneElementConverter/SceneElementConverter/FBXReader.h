#pragma once
#include <string>
#include <fbxsdk.h>
#include "include\DataTypes.h"

class FBXReader
{
public:
	bool ReadFile(const std::string& file, std::vector<MeshData>& meshArr, std::vector<Skeleton>& skeletonArr, std::string& error)const;

private:

	void GetChildInfo(fbxsdk::FbxNode* pNode, fbxsdk::FbxNode*& meshNode, std::vector<MeshData>& meshArr, std::vector<Skeleton>& skeletonArr, bool parent = false)const;
	void GetSkeleton(fbxsdk::FbxNode* pNode, Skeleton& skeleton)const;
	void GetMesh(fbxsdk::FbxNode* pNode, MeshData& mesh)const;

	void AddNewVertex(MeshData& data, VertexData vertex, int FBXIndex)const;
	void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, Vector3<float>& outNormal)const;
	void GetSkinWeightData(fbxsdk::FbxNode* pNode, MeshData& mesh, Skeleton& skeleton)const;
	int FindJointIndexUsingName(Skeleton& skeleton, const std::string& name)const;
	FbxAMatrix GetGeometryTransformation(FbxNode * inNode) const;

};

Matrix4f FBXToMatrix4f(const fbxsdk::FbxAMatrix& matrix);

