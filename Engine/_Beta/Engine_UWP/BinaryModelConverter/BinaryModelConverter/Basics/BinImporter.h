#pragma once
#include "DataTypes.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>

class BMImporter
{
public:
	bool Import(std::string filepath, MeshData& newMesh);
	bool Import(std::vector<unsigned char> data, MeshData& newMesh);

private:

};

inline bool BMImporter::Import(std::vector<unsigned char> data, MeshData& newMesh)
{
	int totalSize = sizeof(int) * 2;
	if (data.empty() || data.size() < totalSize)
		return false;

	VertexData newVertex;
	unsigned int index;
	int offset = 0;

	memcpy(&newMesh.m_indexCount, &data.begin() + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&newMesh.m_vertexCount, &data.begin() + offset, sizeof(int));
	offset += sizeof(int);
	totalSize += newMesh.m_indexCount * sizeof(unsigned int);
	totalSize += newMesh.m_vertexCount * sizeof(VertexData);

	if (data.size() < totalSize)
		return false;

	for (int it = 0; it < newMesh.m_indexCount; it++)
	{
		memcpy(&index, &data.begin() + offset, sizeof(unsigned int));
		offset += sizeof(unsigned int);
		newMesh.m_indices.push_back(index);
	}

	for (int it = 0; it < newMesh.m_vertexCount; it++)
	{
		memcpy(&index, &data.begin() + offset, sizeof(VertexData));
		offset += sizeof(VertexData);
		newMesh.m_vertices.push_back(newVertex);
	}
	return true;
}

inline bool BMImporter::Import(std::string filepath, MeshData& newMesh)
{
	struct stat results;
	if (!stat(filepath.c_str(), &results) == 0)
		return false;

	VertexData newVertex;
	unsigned int index;
	std::ifstream myFile(filepath.c_str(), std::ios::in | std::ios::binary);
	if (myFile)
	{
		if (!myFile.read((char*)&newMesh.m_indexCount, sizeof(int)))
			return false;

		if (!myFile.read((char*)&newMesh.m_vertexCount, sizeof(int)))
			return false;

		for (int it = 0; it < newMesh.m_indexCount; it++)
		{
			if (!myFile.read((char*)&index, sizeof(unsigned int)))
				return false;

			newMesh.m_indices.push_back(index);
		}

		for (int it = 0; it < newMesh.m_vertexCount; it++)
		{
			if (!myFile.read((char*)&newVertex, sizeof(VertexData)))
				return false;

			newMesh.m_vertices.push_back(newVertex);
		}

		myFile.close();
	}
	return true;
}
