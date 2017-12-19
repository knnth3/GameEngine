#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;

template <typename T>
struct Vector3
{
	bool operator==(const Vector3<T>& v)
	{
		if (this->x == v.x && this->y == v.y && this->z == v.z)
		{
			return true;
		}
		return false;
	}

	T x;
	T y;
	T z;
};

struct VertexData
{
	bool operator==(const VertexData& v)
	{
		auto pos = this->m_position == v.m_position;
		auto normals = this->m_normal == v.m_normal;
		auto tangents = this->m_tangent == v.m_tangent;
		auto bitangents = this->m_binormal == v.m_binormal;
		auto textureCoords = this->m_uv == v.m_uv;

		if (pos && normals && tangents && bitangents && textureCoords)
		{
			return true;
		}
		return false;
	}

	Vector3<float> m_position;
	Vector3<float> m_uv;
	Vector3<float> m_normal;
	Vector3<float> m_tangent;
	Vector3<float> m_binormal;
};

struct MeshData
{
	bool operator==(const MeshData& data)
	{
		if (data.m_vertexSize != this->m_vertexSize)
			return false;

		if (data.m_indexSize != this->m_indexSize)
			return false;

		for (int it = 0; it < this->m_vertexSize; it++)
		{
			if (!(this->m_vertices[it] == data.m_vertices[it]))
			{
				return false;
			}
		}

		for (int it = 0; it < this->m_indexSize; it++)
		{
			if (!(this->m_indices[it] == data.m_indices[it]))
			{
				return false;
			}
		}

		return true;
	}
	int m_indexSize;
	int m_vertexSize;
	std::vector<unsigned int> m_indices;
	std::vector<VertexData> m_vertices;
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

void SaveNewData(const std::string& pFile, MeshData mesh)
{
	size_t index = pFile.find_first_of('.');
	auto newName = pFile.substr(0, index);

	std::ofstream ofile(newName + ".bin", std::ios::binary);
	if (ofile)
	{
		ofile.write((char*)&mesh.m_indexSize,  sizeof(int));
		ofile.write((char*)&mesh.m_vertexSize, sizeof(int));

		for (int i = 0; i < mesh.m_indexSize; i++)
		{
			ofile.write((char*)&mesh.m_indices[i], sizeof(unsigned int));
		}

		for (int i = 0; i < mesh.m_vertexSize; i++)
		{
			ofile.write((char*)&mesh.m_vertices[i], sizeof(VertexData));
		}
		ofile.close();
	}
}

void DoTheSceneProcessing(const std::string& pFile,const aiScene* scene, MeshData& m_mesh)
{
	VertexData newVertex;
	vector<vector<int>> m_indices;
	CopyNodesWithMeshes(scene->mRootNode, m_indices);
	for (size_t x = 0; x < m_indices.size(); x++)
	{
		auto mesh = scene->mMeshes[m_indices[x][0]];
		auto name = mesh->mName;
		auto positions = mesh->HasPositions();
		auto uv = mesh->HasTextureCoords(m_indices[x][0]);
		auto normals = mesh->HasNormals();
		auto tangents = mesh->HasTangentsAndBitangents();

		if (normals && tangents && uv && positions)
		{
			for (unsigned int index = 0; index < mesh->mNumFaces; index++)
			{
				auto face = mesh->mFaces[index];

				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					m_mesh.m_indices.push_back(face.mIndices[j]);
				}
			}

			for (unsigned int y = 0; y < mesh->mNumVertices; y++)
			{
				auto pos = mesh->mVertices[y];
				auto norm = mesh->mNormals[y];
				auto bita = mesh->mBitangents[y];
				auto tan = mesh->mTangents[y];
				auto uvs = mesh->mTextureCoords[0][y];

				newVertex.m_position.x = pos.x;
				newVertex.m_position.y = pos.y;
				newVertex.m_position.z = pos.z;

				newVertex.m_normal.x = norm.x;
				newVertex.m_normal.y = norm.y;
				newVertex.m_normal.z = norm.z;

				newVertex.m_tangent.x = tan.x;
				newVertex.m_tangent.y = tan.y;
				newVertex.m_tangent.z = tan.z;

				newVertex.m_binormal.x = bita.x;
				newVertex.m_binormal.y = bita.y;
				newVertex.m_binormal.z = bita.z;

				newVertex.m_uv.x = uvs.x;
				newVertex.m_uv.y = uvs.y;
				newVertex.m_uv.z = uvs.z;

				m_mesh.m_vertices.push_back(newVertex);
			}
		}

		m_mesh.m_indexSize = m_mesh.m_indices.size();
		m_mesh.m_vertexSize = m_mesh.m_vertices.size();
	}
	cout << m_mesh.m_vertexSize << " vertices read" << endl;
	cout << m_mesh.m_indexSize << " indices read" << endl;
	SaveNewData(pFile, m_mesh);
}

bool DoTheImportThing(const std::string& pFile, MeshData& mesh)
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
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		DoTheErrorLogging(importer.GetErrorString());
		return false;
	}
	// Now we can access the file's contents. 
	DoTheSceneProcessing(pFile, scene, mesh);
	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

void DoTheUnitTestThing(std::string filename, MeshData& mesh)
{
	struct stat results;
	if (!stat(filename.c_str(), &results) == 0)
		return;

	VertexData newVertex;
	unsigned int index;
	ifstream myFile(filename.c_str(), ios::in | ios::binary);
	if (myFile)
	{
		if (!myFile.read((char*)&mesh.m_indexSize, sizeof(int)))
			return;

		if (!myFile.read((char*)&mesh.m_vertexSize, sizeof(int)))
			return;

		for (int it = 0; it < mesh.m_indexSize; it++)
		{
			if (!myFile.read((char*)&index, sizeof(unsigned int)))
				return;

			mesh.m_indices.push_back(index);
		}

		for (int it = 0; it < mesh.m_vertexSize; it++)
		{
			if (!myFile.read((char*)&newVertex, sizeof(VertexData)))
				return;

			mesh.m_vertices.push_back(newVertex);
		}

		myFile.close();
	}
}

int main()
{
	//std::string file1 = "Models/stick_figure.3DS";
	//std::string converted1 = "Models/stick_figure.bin";

	//MeshData mesh1;
	//MeshData mesh2;
	//DoTheImportThing(file1, mesh1);
	//DoTheUnitTestThing(converted1, mesh2);

	//if (mesh1 == mesh2)
	//{
	//	cout << "Both files are the same!" << endl;
	//}
	//else
	//{
	//	cout << "Mismatch!" << endl;
	//}

	MeshData mesh3;
	MeshData mesh4;

	std::string file2 = "Models/Cube.obj";
	std::string converted2 = "Models/Cube.bin";

	DoTheImportThing(file2, mesh3);
	DoTheUnitTestThing(converted2, mesh4);

	if (mesh3 == mesh4)
	{
		cout << "Both files are the same!" << endl;
	}
	else
	{
		cout << "Mismatch!" << endl;
	}

	system("pause");
	return 0;
}