#pragma once
#include <string>
#include <fbxsdk.h>
#include "include\DataTypes.h"

class FBX_READER
{
public:
	bool LoadModel(const std::string& file, MeshData& mesh, const std::string& ext, std::string& error)const;
	bool SaveNewMesh(const std::string& file, MeshData mesh)const;

private:

	void AddNewVertex(MeshData& data, VertexData vertex, int FBXIndex)const;
	void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, Vector3<float>& outNormal)const;
	void GetMesh(fbxsdk::FbxNode* pNode, std::vector<MeshData>& meshes)const;
	void GetVertexSkeletonInfo(fbxsdk::FbxNode* pNode, std::vector<MeshData>& meshes, Skeleton& skeleton);
	void GetSkeleton(fbxsdk::FbxNode* pNode, Skeleton& skeleton)const;
	int FindJointIndexUsingName(Skeleton& skeleton, const std::string& name)const;
	FbxAMatrix GetGeometryTransformation(FbxNode * inNode) const;
	std::string to_str(std::wstring original)const;
	std::wstring to_wstr(std::string original)const;

};

