#include "pch.h"
#include "DungeonMapTileset.h"
#include <fstream>
#include <iterator>
#include <sys/stat.h>

using namespace std;
#define SIZEOF_DMT_HEADER 4 * sizeof(uint32_t)
#define SIZEOF_OBJECT3D_HEADER sizeof(uint32_t)
#define LATEST_DMT_VERSION 2

#if PLATFORM == OS_WINDOWS
#endif;

bool FileManager::WriteFile(const std::string filepath, const DMT& outFile)
{
	ofstream file;
	file.open(filepath, ios::out | ios::binary);
	if (!file)
	{
		//Grab the folder path
		size_t found = filepath.find_last_of("/\\");
		if (found > 0)
		{
			string path = filepath.substr(0, found);
			wstring wide(path.begin(), path.end());

#if PLATFORM == OS_WINDOWS
#include <Lime\WindowsAdditionals.h>

			//Try to create the directory (it might not exist)
			if (CreateDirectory(wide.c_str(), NULL))
			{
				file.open(filepath, ios::out | ios::binary);
				if (!file)
					return false;
			}
			else
			{
				auto result = GetLastErrorAsString();
				return false;
			}
#endif
		}
	}

	//Wite DMT header to file
	if (!file.write((const char*)&outFile, SIZEOF_DMT_HEADER))
	{
		file.close();
		return false;
	}

	//Write object data to file
	for (auto x : outFile.objects)
	{
		if (!file.write((const char*)&x, SIZEOF_OBJECT3D_HEADER))
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

	//Write instance data to file
	for (auto x : outFile.instances)
	{
		if (!file.write((const char*)&x, sizeof(x)))
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

	//Check the version before reading
	if (inFile.version != LATEST_DMT_VERSION)
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

	//Load in Instances
	for (size_t x = 0; x < inFile.nInstances; x++)
	{
		ObjInstance instance;
		if (!file.read((char*)&instance, sizeof(instance)))
		{
			file.close();
			return false;
		}
		inFile.instances.push_back(instance);
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
	std::unordered_map<MeshID, uint32_t> addedMeshes;
	std::unordered_map<Lime::TextureID, uint32_t> addedTextures;
	//Convert all the models into objects
	for (auto x : data)
	{
		ObjInstance inst;
		MeshID currentMesh = x->GetMesh();
		inst.position = x->GetPosition();
		inst.scale = x->GetScale();
		Lime::TextureID currentTexture = x->GetTexture();
		if (currentTexture == -1)
		{
			bool bAdded = false;
		}
		bool bAdded = false;
		//Add objects
		for (auto y : addedMeshes)
		{
			if (y.first == currentMesh)
			{
				inst.objectID = y.second;
				bAdded = true;
				break;
			}
		}
		if (!bAdded)
		{
			Object_3D obj;
			x->GetMeshData(obj.vertices, obj.indices);
			obj.nVertices = (uint32_t)obj.vertices.size();
			inst.objectID = (uint32_t)objects.size();
			addedMeshes.emplace(currentMesh, (uint32_t)objects.size());
			objects.push_back(obj);
			bAdded = false;
		}
		bAdded = false;

		//Add textures
		for (auto y : addedTextures)
		{
			if (y.first == currentTexture)
			{
				inst.textureID = y.second;
				bAdded = true;
				break;
			}
		}
		if (!bAdded)
		{
			inst.textureID = (uint32_t)textures.size();
			addedTextures.emplace(currentTexture, (uint32_t)textures.size());
			textures.push_back(Lime::TextureManager::GetFilePath(x->GetTexture()));
			bAdded = false;
		}
		bAdded = false;

		//Add polled instance
		instances.push_back(inst);
	}
	//Set header
	nTextures = (uint32_t)textures.size();
	nInstances = (uint32_t)instances.size();
	nObjects = (uint32_t)objects.size();
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

size_t Object_3D::vertSize()
{
	return sizeof(Lime::Model::Vertex) * vertices.size();
}

size_t Object_3D::indicesSize()
{
	return sizeof(uint32_t) * indices.size();
}
