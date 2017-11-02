#include "DungeonMapTileset.h"
#include <fstream>
#include <iterator>
#include <sys/stat.h>

using namespace std;
#define SIZEOF_DMT_HEADER 3 * sizeof(uint32_t)
#define SIZEOF_OBJECT3D_HEADER sizeof(uint32_t) + (2 * sizeof(glm::vec3))
#define LATEST_DMT_VERSION 1

bool FileManager::WriteFile(const std::string filepath, const DMT& outFile)
{
	ofstream file;
	file.open(filepath, ios::out | ios::binary);
	if (!file)
		return false;

	//Wite DMT header to file
	if (!file.write((const char*)&outFile, SIZEOF_DMT_HEADER))
	{
		file.close();
		return false;
	}

	//Write object data to file
	for (auto x : outFile.objects)
	{
		if (!file.write((const char*)&x, x.headerSize()))
		{
			file.close();
			return false;
		}

		if (!file.write((const char*)x.vertices.data(), x.vertSize()))
		{
			file.close();
			return false;
		}

		if (!file.write((const char*)x.indices.data(), x.indicesSize()))
		{
			file.close();
			return false;
		}
	}

	//Write texture data to file
	for (auto x : outFile.textures)
	{
		if (!file.write((const char*)&x.size, sizeof(x.size)))
		{
			file.close();
			return false;
		}

		if (!file.write((const char*)x.data.data(), x.size))
		{
			file.close();
			return false;
		}
	}

	file.close();
	return true;
}

bool FileManager::LoadFile(const std::string filepath, DMT& inFile)
{
	//Note: ios flags may act differently depending on the GNU compiler or working with unix
	//Check file if it's a valid size
	struct stat results;
	if (stat(filepath.c_str(), &results) != 0)
		return false;
	if (results.st_size <= SIZEOF_DMT_HEADER)
		return false;

	ifstream file;
	file.open(filepath, ios::in | ios::binary);
	if (!file)
		return false;

	//Read in header information
	if (!file.read((char*)&inFile, SIZEOF_DMT_HEADER))
	{
		file.close();
		return false;
	}

	//Load in Objects
	for (size_t x = 0; x < inFile.nObjects; x++)
	{
		Object_3D obj;
		//Get the next object's header
		if (!file.read((char*)&obj.nVertices, SIZEOF_OBJECT3D_HEADER))
		{
			file.close();
			return false;
		}

		//Get obj's data
		obj.vertices.resize(obj.nVertices);
		obj.indices.resize(obj.nVertices);
		if (!file.read((char*)obj.vertices.data(), obj.nVertices * sizeof(Lime::Model::Vertex)))
		{
			file.close();
			return false;
		}

		if (!file.read((char*)obj.indices.data(), obj.nVertices * sizeof(uint32_t)))
		{
			file.close();
			return false;
		}

		inFile.objects.push_back(obj);
	}

	//Load in textures
	for (size_t x = 0; x < inFile.nTextures; x++)
	{
		DMT_STRING str;
		//Get the string's size
		if (!file.read((char*)&str.size, sizeof(uint32_t)))
		{
			file.close();
			return false;
		}

		str.data.resize(str.size);
		if (!file.read((char*)str.data.data(), str.size))
		{
			file.close();
			return false;
		}
		inFile.textures.push_back(str);
	}

	file.close();
	return true;
}

DMT::DMT()
{
	version = LATEST_DMT_VERSION;
	nObjects = 0;
	nTextures = 0;
}

void DMT::AddData(const std::vector<std::shared_ptr<Lime::Model::Model3D>>& data)
{
	nObjects = (uint32_t)data.size();

	//Convert all the models into objects
	for (auto x : data)
	{
		Object_3D obj;
		x->GetMeshData(obj.vertices, obj.indices);
		obj.nVertices = (uint32_t)obj.vertices.size();
		obj.position = x->GetPosition();
		obj.scale = x->GetScale();
		objects.push_back(obj);
	}

	//Add textures in
	//textures = 
	//{
	//	string("Hello World!"),
	//	string("How are you?")
	//};
	//nTextures = textures.size();
}

size_t DMT::Size()
{
	//Start off with the header's size
	size_t totalSize = sizeof(uint32_t) * 3;

	//Add each object's size to the total
	for (auto x : objects)
		totalSize += x.size();

	//Add each textures size to the total
	for (auto x : textures)
		totalSize += x.size;

	return totalSize;
}

size_t Object_3D::size()
{
	size_t totalSize = 0;
	//indices + nVertices
	totalSize += sizeof(uint32_t) * (indices.size() + 1);
	//2 vec3s
	totalSize += sizeof(glm::vec3) * 2;
	//vertices
	totalSize += sizeof(Lime::Model::Vertex) * vertices.size();

	return totalSize;
}

size_t Object_3D::headerSize()
{
	size_t totalSize = 0;
	//nVertices
	totalSize += sizeof(uint32_t);
	//Position + Scale
	totalSize += sizeof(glm::vec3) * 2;

	return totalSize;
}

size_t Object_3D::vertSize()
{
	return sizeof(Lime::Model::Vertex) * vertices.size();
}

size_t Object_3D::indicesSize()
{
	return sizeof(uint32_t) * indices.size();
}
