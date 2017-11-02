#pragma once
#include "Lime\Model.h"


class DMT
{
public:
	size_t Size();
	/////////Header///////////////
	uint32_t version;
	uint32_t nVertices;
	uint32_t nTextures;
	//////////////////////////////
	std::vector<Lime::Model::Vertex> vertices;
	std::vector<std::string> textures;

};

class FileManager
{
public:
	//Writes data and saves into a .dmt filetype
	//Returns true on success
	static bool WriteFile(std::string filepath, const std::vector<Lime::Model::Vertex>& vertices, const std::vector<std::string>& textures);
	//Reads data from .dmt filetype
	//Return true on success
	static bool LoadFile(std::string filepath, std::vector<Lime::Model::Vertex>& vertices, std::vector<std::string>& textures);
};