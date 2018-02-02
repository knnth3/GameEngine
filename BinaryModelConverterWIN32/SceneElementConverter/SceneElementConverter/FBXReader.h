#pragma once
#include <string>
#include <fbxsdk.h>
#include <unordered_map>
#include "include\DataTypes.h"

struct MeshTempData
{
	int Size;
	std::string Name;
	std::vector<unsigned int> Index;
	std::unordered_map<int, std::vector<std::pair<int, VertexData>>> Library;
};

inline void ExtractMeshDataFromTemp(const MeshTempData& temp, MeshData& data);
inline MeshTempData* FindMeshByName(const std::string& name, std::vector<MeshTempData>& meshArr);

class FBXReader
{
public:
	bool ReadFile(const std::string& file, std::vector<MeshData>& meshArr, std::vector<Skeleton>& skeletonArr, std::string& error)const;

private:

	void GetChildInfo(fbxsdk::FbxNode* pNode, fbxsdk::FbxNode*& meshNode, std::vector<MeshTempData>& meshArr, std::vector<Skeleton>& skeletonArr, bool parent = false)const;
	void GetSkeleton(fbxsdk::FbxNode* pNode, Skeleton& skeleton)const;
	void ReadVertexInfo(fbxsdk::FbxNode* pNode, MeshTempData& mesh)const;

	void AddNewVertex(MeshTempData& data, const VertexData& vertex, int FBXIndex)const;
	void ReadVertexInfo(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, VertexData& outVertex)const;
	bool GetSkinWeightData(fbxsdk::FbxNode* pNode, MeshTempData* mesh, Skeleton& skeleton)const;
	int FindJointIndexUsingName(Skeleton& skeleton, const std::string& name)const;
	FbxAMatrix GetGeometryTransformation(FbxNode * inNode) const;

};

Matrix4f FBXToMatrix4f(const fbxsdk::FbxAMatrix& matrix);

