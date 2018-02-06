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
	std::unordered_map<int, std::vector<std::pair<int, SEF::VertexData>>> Library;
};

inline void ExtractMeshDataFromTemp(const MeshTempData& temp, SEF::MeshData& data);
inline MeshTempData* FindMeshByName(const std::string& name, std::vector<MeshTempData>& meshArr);

class FBXReader
{
public:
	bool ReadFile(const std::string& file, std::vector<SEF::MeshData>& meshArr, std::vector<SEF::Skeleton>& skeletonArr, std::string& error, bool attatchment = false, int startBoneIndexCount = 0);

private:

	void GetChildInfo(fbxsdk::FbxNode* pNode, fbxsdk::FbxNode*& meshNode, std::vector<MeshTempData>& meshArr, std::vector<SEF::Skeleton>& skeletonArr, bool parent = false)const;
	uint32_t GetSkeleton(fbxsdk::FbxNode* pNode, std::vector<SEF::Joint>& joints)const;
	void ReadVertexInfo(fbxsdk::FbxNode* pNode, MeshTempData& mesh)const;

	void AddNewVertex(MeshTempData& data, const SEF::VertexData& vertex, int FBXIndex)const;
	void ReadVertexInfo(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, SEF::VertexData& outVertex)const;
	bool GetSkinWeightData(fbxsdk::FbxNode* pNode, MeshTempData* mesh, SEF::Skeleton& skeleton)const;
	int FindJointIndexUsingName(SEF::Skeleton& skeleton, const std::string& name)const;
	FbxAMatrix GetGeometryTransformation(FbxNode * inNode) const;

	bool m_bAttatchment;
	int m_boneIndexOffset;
	FbxScene* m_FBXScene;
};

SEF::Matrix4f FBXToMatrix4f(const fbxsdk::FbxAMatrix& matrix);

