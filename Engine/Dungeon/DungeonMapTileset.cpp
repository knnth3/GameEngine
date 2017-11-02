#include "DungeonMapTileset.h"
#include <fstream>
#include <iterator>
#include <sys/stat.h>

using namespace std;
#define SIZEOF_DMT_HEADER 3 * sizeof(uint32_t)

bool FileManager::WriteFile(std::string filepath, const std::vector<Lime::Model::Vertex>& vertices, const std::vector<std::string>& textures)
{
	uint32_t vertexSize = vertices.size() * sizeof(Lime::Model::Vertex);
	uint32_t vertSize = vertices.size();
	uint32_t textureSize = textures.size();
	uint32_t version = 1;
	ofstream file;
	file.open(filepath, ios::out | ios::binary);
	if (!file)
		return false;
	//Wite Header
	file.write((const char*)&version, sizeof(uint32_t));
	file.write((const char*)&vertSize, sizeof(uint32_t));
	file.write((const char*)&textureSize, sizeof(uint32_t));
	//Write Vertex data
	file.write((const char*)vertices.data(), vertexSize);
	//Write texture data
	for (auto str : textures)
	{
		file.write(str.c_str(), str.size());
	}
	file.close();
}

bool FileManager::LoadFile(std::string filepath, std::vector<Lime::Model::Vertex>& vertices, std::vector<std::string>& textures)
{
	//Check file if it's a valid size
	struct stat results;
	if (stat(filepath.c_str(), &results) != 0)
		return false;
	if (results.st_size <= SIZEOF_DMT_HEADER)
		return false;

	ifstream file;
	//Note: ios flags may act differently depending on the GNU compiler
	//-or working with unix
	file.open(filepath, ios::in | ios::binary);
	if (!file)
		return false;

	DMT myFile;
	uint32_t offset = 0;
	////Grab the header of the file
	if (!file.read((char*)&myFile, SIZEOF_DMT_HEADER))
		return false;
	offset += SIZEOF_DMT_HEADER;

	//Load in vertices
	if (!file.read((char*)&myFile + offset,
		sizeof(Lime::Model::Vertex) * myFile.nVertices))
		return false;
	offset += sizeof(Lime::Model::Vertex) * myFile.nVertices;

	//Load rest of the bytes
	size_t dataSize = results.st_size - offset;
	std::vector<char>byteData(dataSize, '\0');
	if (!file.read(byteData.data(), byteData.size()))
		return false;

	cout << byteData.data() << endl;
	file.close();
}

size_t DMT::Size()
{
	return (sizeof(uint32_t) * 3) + vertices.size() + textures.size();
}
