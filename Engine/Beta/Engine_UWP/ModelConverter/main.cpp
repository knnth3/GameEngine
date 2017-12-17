#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
struct Vector3
{
	T x;
	T y;
	T z;
};

struct Vertex
{
	Vector3<float> m_position;
	Vector3<float> m_normal;
	Vector3<float> m_tangent;
	Vector3<float> m_bitangent;
};

void DoTheErrorLogging(std::string err)
{
	cout << "Error: " << err << endl;
}

void CopyMeshes(aiNode* node, vector<int>& targetParent)
{
	for (unsigned int a = 0; a < node->mNumMeshes; a++)
	{
		int index = node->mMeshes[a];
		targetParent.push_back(index);
	}
}

void CopyNodesWithMeshes(aiNode* node, vector<vector<int>>& targetParent)
{
	vector<int> parent;
	// if node has meshes, create a new scene object for it
	if (node->mNumMeshes > 0)
	{
		// copy the meshes
		CopyMeshes(node, parent);

		// the new object is the parent for all child nodes
		targetParent.push_back(parent);
	}
	// continue for all child nodes
	for (unsigned int a = 0; a < node->mNumChildren; a++)
		CopyNodesWithMeshes(node->mChildren[a], targetParent);
}

void DoTheSceneProcessing(const aiScene* scene)
{
	vector<vector<Vector3<float>>> m_vertices;
	vector<vector<int>> m_indices;
	CopyNodesWithMeshes(scene->mRootNode, m_indices);
	for (auto x = 0; x < m_indices.size(); x++)
	{
		auto mesh = scene->mMeshes[m_indices[x][0]];
		auto name = mesh->mName;
		auto verts = mesh->mNumVertices;
		auto normals = mesh->HasNormals();
		auto tangents = mesh->HasTangentsAndBitangents();
		cout << "New Mesh!" << endl;
	}
}

bool DoTheImportThing(const std::string& pFile)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_MakeLeftHanded);

	// If the import failed, report it
	if (!scene)
	{
		DoTheErrorLogging(importer.GetErrorString());
		return false;
	}
	// Now we can access the file's contents. 
	DoTheSceneProcessing(scene);
	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

int main()
{
	std::string file = "Character1/Stick_Figure_by_Swp.3DS";
	DoTheImportThing(file);
	system("pause");
	return 0;
}