#pragma once
#include "Lime\Model3D.h"


struct Object_3D
{
	size_t size();
	size_t vertSize();
	size_t indicesSize();

	uint32_t nVertices;
	std::vector<Lime::Model::Vertex> vertices;
	std::vector<uint32_t> indices;
};
struct ObjInstance
{
	uint32_t objectID;
	uint32_t textureID;
	glm::vec3 position;
	glm::vec3 scale;
};

struct DMT_STRING
{
	DMT_STRING() = default;
	DMT_STRING(std::string in)
	{
		size = (uint32_t)in.size();
		data = in;
	}
	uint32_t size;
	std::string data;
};

class DMT
{
public:
	DMT();
	void AddData(const std::vector<std::shared_ptr<Lime::Model::Model3D>>& data);
	size_t Size();
	/////////Header///////////////
	uint32_t version;
	uint32_t nObjects;
	uint32_t nInstances;
	uint32_t nTextures;
	//////////////////////////////
	std::vector<Object_3D> objects;
	std::vector<ObjInstance> instances;
	std::vector<DMT_STRING> textures;

};

class FileManager
{
public:
	//Writes data and saves into a .dmt filetype
	//Returns true on success
	static bool WriteFile(const std::string filepath, const DMT& outFile);
	//Reads data from .dmt filetype
	//Return true on success
	static bool LoadFile(const std::string filepath, DMT& inFile);
};