#pragma once
#include <string>
#include <fbxsdk.h>
#include "Basics\DataTypes.h"

class FBX_READER
{
public:
	bool LoadModel(Windows::Storage::StorageFile^ file, MeshData& mesh, Platform::String^ ext, Platform::String^& error)const;
	bool SaveNewMesh(Windows::Storage::StorageFile^ file, MeshData mesh)const;

private:

	void AddNewVertex(MeshData& data, VertexData vertex)const;
	void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, Vector3<float>& outNormal)const;
	void PrintNode(fbxsdk::FbxNode* pNode, std::vector<MeshData>& meshes)const;
	std::string to_str(std::wstring original)const;
	std::wstring to_wstr(std::string original)const;

};

