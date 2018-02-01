#pragma once

//Scene Element File
#include "DataTypes.h"
#include <fstream>

#define FILE_EXTENSION ".sef"
#define CURRENT_SEF_VERSION 1
#define MAX_SEF_HEADER_STR 16

struct SEF_HEADER
{
	uint16_t Version;
	char Name[MAX_SEF_HEADER_STR];
	char Author[MAX_SEF_HEADER_STR];
	bool bHasSkeleton;
	bool bHasMesh;
};

struct SEF_MESH_HEADER
{
	uint32_t Indices;
	uint32_t Vertices;
	uint16_t NameLength;
};

inline SEF_HEADER CreateHeader(uint16_t version = -1, bool hasMesh = false,
	bool hasSkeleton = false, const char* name = nullptr, const char* author = nullptr);
inline std::string GetExtension(const std::string& filepath);
inline int GetStrSize(const char* str);

class SEStream
{
public:

	int WriteFile(const std::string& filename, const MeshData* mesh, const Skeleton* skeleton)const
	{
		int bytesWritten = 0;
		std::ofstream myFile(filename.c_str(), std::ios::out | std::ios::binary);
		SEF_HEADER header = CreateHeader(CURRENT_SEF_VERSION, mesh, skeleton, "Model", "Online Author");
		try
		{
			myFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			//Write file header first
			myFile.write((char*)&header, sizeof(header));
			bytesWritten += sizeof(header);

			if (mesh)
			{
				//Header
				SEF_MESH_HEADER meshInfo;
				meshInfo.Indices = (uint32_t)mesh->Indices.size();
				meshInfo.Vertices = (uint32_t)mesh->Vertices.size();
				meshInfo.NameLength = (uint16_t)mesh->Name.length();
				myFile.write((char*)&meshInfo, sizeof(meshInfo));
				bytesWritten += sizeof(meshInfo);

				//Name
				std::string name = mesh->Name;
				myFile.write(name.data(), sizeof(char) * meshInfo.NameLength);
				bytesWritten += sizeof(char) * meshInfo.NameLength;

				//Vertices
				for (const auto& vert : mesh->Vertices)
				{
					myFile.write((char*)&vert, sizeof(vert));
					bytesWritten += sizeof(vert);
				}

				//Indices
				for (const auto& index : mesh->Indices)
				{
					myFile.write((char*)&index, sizeof(index));
					bytesWritten += sizeof(index);
				}

			}

			myFile.close();
		}
		catch (const std::exception e)
		{
			std::string error = "Unable to write to file " + filename + "\nReason: " + e.what();
			throw std::runtime_error(error);
		}
		return bytesWritten;
	}

	int ReadFile(const std::string& filename, MeshData* mesh, Skeleton* skeleton)const
	{
		int bytesRead = 0;
		std::string ext = GetExtension(filename);
		if (ext.compare(FILE_EXTENSION))
		{
			return false;
		}

		std::ifstream myFile(filename.c_str(), std::ios::out | std::ios::binary);
		SEF_HEADER header;
		try
		{
			myFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			//Read file header first
			myFile.read((char*)&header, sizeof(header));
			bytesRead += sizeof(header);

			if (header.bHasMesh && mesh)
			{
				//Header
				SEF_MESH_HEADER meshInfo;
				myFile.read((char*)&meshInfo, sizeof(meshInfo));
				bytesRead += sizeof(meshInfo);

				//Name
				mesh->Name.resize(meshInfo.NameLength);
				myFile.read(&mesh->Name[0], sizeof(char) * meshInfo.NameLength);
				bytesRead += sizeof(char) * meshInfo.NameLength;

				//Vertices
				for (uint32_t x = 0; x < meshInfo.Vertices; x++)
				{
					VertexData vert;
					myFile.read((char*)&vert, sizeof(vert));
					bytesRead += sizeof(vert);

					mesh->Vertices.push_back(vert);
				}

				//Index
				for (uint32_t x = 0; x < meshInfo.Indices; x++)
				{
					unsigned int index;
					myFile.read((char*)&index, sizeof(index));
					bytesRead += sizeof(index);

					mesh->Indices.push_back(index);
				}

			}

			myFile.close();
		}
		catch (const std::exception e)
		{
			std::string error = "Unable to read file " + filename + "\nReason: " + e.what();
			throw std::runtime_error(error);
		}
		return bytesRead;
	}

};

inline SEF_HEADER CreateHeader(uint16_t version, bool hasMesh, bool hasSkeleton, const char* name, const char* author)
{
	SEF_HEADER header;
	header.Version = version;
	header.bHasMesh = hasMesh;
	header.bHasSkeleton = hasSkeleton;

	int nameSize = GetStrSize(name);
	int range = (nameSize > MAX_SEF_HEADER_STR - 1) ? MAX_SEF_HEADER_STR - 1 : nameSize;
	for (int x = 0; x < MAX_SEF_HEADER_STR; x++)
	{
		if (x >= range)
			header.Name[x] = '\0';
		else
			header.Name[x] = name[x];
	}

	int authorSize = GetStrSize(author);
	range = (authorSize > MAX_SEF_HEADER_STR - 1) ? MAX_SEF_HEADER_STR - 1 : authorSize;
	for (int x = 0; x < MAX_SEF_HEADER_STR; x++)
	{
		if (x >= range)
			header.Author[x] = '\0';
		else
			header.Author[x] = author[x];
	}

	return header;
}

inline std::string GetExtension(const std::string& filepath)
{
	auto result = filepath.find_last_of('.');
	if (result == std::string::npos)
		return std::string();

	return filepath.substr(result, std::string::npos);
}

inline int GetStrSize(const char * str)
{
	int size = 0;
	if (str)
	{
		char current = str[size];
		while (current != '\0')
		{
			size++;
			current = str[size];
		}
	}
	return size;
}
