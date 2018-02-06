#pragma once
#include "DataTypes.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>

class BMImporter
{
public:
	bool Import(std::string filepath, MeshData& newMesh);

private:

};

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
		if (!myFile.read((char*)&newMesh.m_indexSize, sizeof(int)))
			return false;

		if (!myFile.read((char*)&newMesh.m_vertexSize, sizeof(int)))
			return false;

		for (int it = 0; it < newMesh.m_indexSize; it++)
		{
			if (!myFile.read((char*)&index, sizeof(unsigned int)))
				return false;

			newMesh.m_indices.push_back(index);
		}

		for (int it = 0; it < newMesh.m_vertexSize; it++)
		{
			if (!myFile.read((char*)&newVertex, sizeof(VertexData)))
				return false;

			newMesh.m_vertices.push_back(newVertex);
		}

		myFile.close();
	}
	return true;
}
