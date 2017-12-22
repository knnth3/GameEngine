#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "BinImporter.h"

using namespace std;

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

bool DoTheSceneProcessing(const std::string& pFile,const aiScene* scene, MeshData& m_mesh)
{
	VertexData newVertex;
	vector<vector<int>> m_indices;
	CopyNodesWithMeshes(scene->mRootNode, m_indices);
	if (m_indices.empty())
		return false;

	for (size_t x = 0; x < m_indices.size(); x++)
	{
		auto mesh = scene->mMeshes[m_indices[x][0]];
		auto name = mesh->mName;
		auto positions = mesh->HasPositions();
		auto uv = mesh->HasTextureCoords(m_indices[x][0]);
		auto normals = mesh->HasNormals();
		auto tangents = mesh->HasTangentsAndBitangents();
		auto colors = mesh->HasVertexColors(m_indices[x][0]);

		bool bHasLook = (colors || uv);
		if (normals && positions && bHasLook)
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

				newVertex.m_position.x = pos.x;
				newVertex.m_position.y = pos.y;
				newVertex.m_position.z = pos.z;

				newVertex.m_normal.x = norm.x;
				newVertex.m_normal.y = norm.y;
				newVertex.m_normal.z = norm.z;

				if (tangents)
				{
					auto bita = mesh->mBitangents[y];
					auto tan = mesh->mTangents[y];

					newVertex.m_bHasTangents = true;
					newVertex.m_tangent.x = tan.x;
					newVertex.m_tangent.y = tan.y;
					newVertex.m_tangent.z = tan.z;

					newVertex.m_binormal.x = bita.x;
					newVertex.m_binormal.y = bita.y;
					newVertex.m_binormal.z = bita.z;
				}

				if (colors)
				{
					auto color = mesh->mColors[0][y];
					newVertex.m_color.x = color.r;
					newVertex.m_color.y = color.g;
					newVertex.m_color.z = color.b;
				}
				else if (uv)
				{
					auto uvs = mesh->mTextureCoords[0][y];
					newVertex.m_uv.x = uvs.x;
					newVertex.m_uv.y = uvs.y;
					newVertex.m_uv.z = uvs.z;
				}

				m_mesh.m_vertices.push_back(newVertex);
			}
		}

		m_mesh.m_indexSize = m_mesh.m_indices.size();
		m_mesh.m_vertexSize = m_mesh.m_vertices.size();
	}
	cout << m_mesh.m_vertexSize << " vertices read" << endl;
	cout << m_mesh.m_indexSize << " indices read" << endl;
	SaveNewData(pFile, m_mesh);
	return true;
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
	// We're done. Everything will be cleaned up by the importer destructor
	return 	DoTheSceneProcessing(pFile, scene, mesh);
}

std::string RemoveExtension(std::string original)
{
	return original.substr(0, original.find_first_of('.'));
}

int main()
{
	BMImporter io;
	vector<string> files;

	//files.push_back("Models/Plane.obj");
	//files.push_back("Models/Cube.obj");
	files.push_back("Models/Characters/Models/Robe/body_robe_gold_common.fbx");

	for (auto f : files)
	{
		cout << "------Processing: " + f + "------" << endl;
		MeshData original;
		MeshData bin;
		if (DoTheImportThing(f, original))
		{
			std::string path = RemoveExtension(f);
			io.Import(path + ".bin", bin);

			if (original == bin)
			{
				cout << endl;
				cout << "**Conversion Successful!**" << endl;
				cout << "----------------------------" << endl;
			}
			else
			{
				cout << endl;
				cout << "Conversion falied: Unsupported bin file." << endl;
				cout << "----------------------------" << endl;
			}
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "Conversion falied: Invalid input file/location." << endl;
			cout << "----------------------------" << endl;
		}

	}

	system("pause");
	return 0;
}