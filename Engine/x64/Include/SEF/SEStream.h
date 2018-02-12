#pragma once

//Scene Element File
#include "Animation.h"
#include <fstream>
#include <queue>

#define FILE_EXTENSION ".sef"
#define CURRENT_SEF_VERSION 4
#define MAX_SEF_HEADER_STR 16

namespace SEF
{

	struct SEF_HEADER
	{
		uint16_t Version;
		char Name[MAX_SEF_HEADER_STR];
		char Author[MAX_SEF_HEADER_STR];
		bool bHasSkeleton;
		bool bHasMesh;
		uint16_t AnimCount;
	};

	struct SEF_MESH_HEADER
	{
		uint32_t Indices;
		uint32_t Vertices;
		uint16_t NameLength;
	};

	struct SEF_SKELETON_HEADER
	{
		uint32_t Joints;
		uint16_t NameLength;
	};

	struct SEF_ANIMATION_HEADER
	{
		uint32_t Joints;
		uint32_t Timestamps;
	};

	inline SEF_HEADER CreateHeader(uint16_t version = -1, uint16_t animCount = 0, bool hasMesh = false,
		bool hasSkeleton = false, const char* name = nullptr, const char* author = nullptr);
	inline std::string GetExtension(const std::string& filepath);
	inline int GetStrSize(const char* str);

	class SEStream
	{
	public:

		void AddAnimation(const Animation& anim);
		int WriteFile(const std::string& filename, const MeshData* mesh, const Skeleton* skeleton);
		int ReadFile(const std::string& filename, MeshData* mesh, Skeleton* skeleton, std::vector<Animation>* animation)const;

	private:
		std::queue<Animation> m_animations;

	};

	inline SEF_HEADER CreateHeader(uint16_t version, uint16_t animCount, bool hasMesh, bool hasSkeleton, const char* name, const char* author)
	{
		SEF_HEADER header;
		header.Version = version;
		header.bHasMesh = hasMesh;
		header.bHasSkeleton = hasSkeleton;
		header.AnimCount = animCount;

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


	void SEStream::AddAnimation(const Animation & anim)
	{
		m_animations.push(anim);
	}

	inline int SEStream::WriteFile(const std::string & filename, const MeshData * mesh, const Skeleton * skeleton)
	{
		int bytesWritten = 0;
		std::ofstream myFile(filename.c_str(), std::ios::out | std::ios::binary);
		SEF_HEADER header = CreateHeader(CURRENT_SEF_VERSION, (uint16_t)m_animations.size(), mesh, skeleton, "Model", "Online Author");
		try
		{
			myFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			//Write file header first
			myFile.write((char*)&header, sizeof(header));
			bytesWritten += sizeof(header);

			if (mesh)
			{
				//Header
				SEF_MESH_HEADER meshInfo = {};
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

			if (skeleton)
			{
				//Header
				SEF_SKELETON_HEADER skeletonInfo = {};
				skeletonInfo.NameLength = (uint16_t)skeleton->SkinName.size();
				skeletonInfo.Joints = (uint32_t)skeleton->Joints.size();
				myFile.write((char*)&skeletonInfo, sizeof(skeletonInfo));
				bytesWritten += sizeof(skeletonInfo);

				//Name
				std::string name = skeleton->SkinName;
				myFile.write(name.data(), sizeof(char) * skeletonInfo.NameLength);
				bytesWritten += sizeof(char) * skeletonInfo.NameLength;

				//Joints
				for (const auto& joint : skeleton->Joints)
				{
					//Joint name length
					uint16_t nameLength = (uint16_t)joint.Name.size();
					myFile.write((char*)&nameLength, sizeof(nameLength));
					bytesWritten += sizeof(nameLength);

					//Joint name
					std::string jname = joint.Name;
					myFile.write(jname.data(), sizeof(char) * nameLength);
					bytesWritten += sizeof(char) * nameLength;

					//Joint matrix
					myFile.write((char*)&joint.GlobalBindPoseInverse, sizeof(joint.GlobalBindPoseInverse));
					bytesWritten += sizeof(joint.GlobalBindPoseInverse);

					//Joint children count
					uint32_t ccount = (uint32_t)joint.Children.size();
					myFile.write((char*)&ccount, sizeof(ccount));
					bytesWritten += sizeof(ccount);

					//Joint children
					for (const auto& child : joint.Children)
					{
						myFile.write((char*)&child, sizeof(child));
						bytesWritten += sizeof(child);
					}
				}
			}

			while(!m_animations.empty())
			{
				auto animation = m_animations.front();
				m_animations.pop();

				SEF_ANIMATION_HEADER animInfo;
				animInfo.Timestamps = (uint32_t)animation.TimeStampCount();
				animInfo.Joints = (uint32_t)animation.JointCount();
				myFile.write((char*)&animInfo, sizeof(animInfo));
				bytesWritten += sizeof(animInfo);

				for (const auto& name : animation.JointNames())
				{
					uint16_t size = (uint16_t)name.size();
					myFile.write((char*)&size, sizeof(size));
					bytesWritten += sizeof(size);

					myFile.write(name.data(), sizeof(char) * size);
					bytesWritten += sizeof(char) * size;
				}

				for (int index = 0; index < (int)animInfo.Joints; index++)
				{
					auto* cluster = animation.At(index);
					for (int keyIndex = 0; keyIndex < (int)animInfo.Timestamps; keyIndex++)
					{
						const auto& key = cluster->at(keyIndex);
						myFile.write((char*)&key, sizeof(key));
						bytesWritten += sizeof(key);
					}
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

	inline int SEStream::ReadFile(const std::string & filename, MeshData * mesh, Skeleton * skeleton, std::vector<Animation>* animation) const
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
				SEF_MESH_HEADER meshInfo = {};
				myFile.read((char*)&meshInfo, sizeof(meshInfo));
				bytesRead += sizeof(meshInfo);

				//Name
				mesh->Name.resize(meshInfo.NameLength);
				myFile.read(&mesh->Name[0], sizeof(char) * meshInfo.NameLength);
				bytesRead += sizeof(char) * meshInfo.NameLength;

				//Vertices
				for (uint32_t x = 0; x < meshInfo.Vertices; x++)
				{
					VertexData vert = {};
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

			if (header.bHasSkeleton && skeleton)
			{
				//Header
				SEF_SKELETON_HEADER skeletonInfo = {};
				skeletonInfo.NameLength = (uint16_t)skeleton->SkinName.size();
				skeletonInfo.Joints = (uint32_t)skeleton->Joints.size();
				myFile.read((char*)&skeletonInfo, sizeof(skeletonInfo));
				bytesRead += sizeof(skeletonInfo);

				//Name
				skeleton->SkinName.resize(skeletonInfo.NameLength);
				myFile.read(&skeleton->SkinName[0], sizeof(char) * skeletonInfo.NameLength);
				bytesRead += sizeof(char) * skeletonInfo.NameLength;

				//Joints
				for (uint32_t x = 0; x < skeletonInfo.Joints; x++)
				{
					Joint joint = {};
					//Joint name length
					uint16_t nameLength = 0;
					myFile.read((char*)&nameLength, sizeof(nameLength));
					bytesRead += sizeof(nameLength);

					//Joint name
					joint.Name.resize(nameLength);
					myFile.read(&joint.Name[0], sizeof(char) * nameLength);
					bytesRead += sizeof(char) * nameLength;

					//Joint matrix
					myFile.read((char*)&joint.GlobalBindPoseInverse, sizeof(joint.GlobalBindPoseInverse));
					bytesRead += sizeof(joint.GlobalBindPoseInverse);

					//Joint children count
					uint32_t ccount = 0;
					myFile.read((char*)&ccount, sizeof(ccount));
					bytesRead += sizeof(ccount);

					//Joint children
					joint.Children.resize(ccount);
					for (auto& child : joint.Children)
					{
						myFile.read((char*)&child, sizeof(child));
						bytesRead += sizeof(child);
					}

					skeleton->Joints.push_back(joint);
				}
			}

			while (header.AnimCount && animation)
			{
				header.AnimCount--;
				animation->emplace_back();
				auto& anim = animation->back();
				SEF_ANIMATION_HEADER animInfo;
				myFile.read((char*)&animInfo, sizeof(animInfo));
				bytesRead += sizeof(animInfo);

				anim.SetTimestampCount(animInfo.Timestamps);
				std::vector<std::string> jointNames;
				jointNames.resize(animInfo.Joints);
				for (auto& name : jointNames)
				{
					uint16_t size = 0;
					myFile.read((char*)&size, sizeof(size));
					bytesRead += sizeof(size);

					name.resize(size);
					myFile.read(&name[0], sizeof(char) * size);
					bytesRead += sizeof(char) * size;
				}

				for (int index = 0; index < (int)animInfo.Joints; index++)
				{
					std::vector<AnimationKey> cluster;
					cluster.resize(animInfo.Timestamps);
					for (int keyIndex = 0; keyIndex < (int)animInfo.Timestamps; keyIndex++)
					{
						const auto& key = cluster.at(keyIndex);
						myFile.read((char*)&key, sizeof(key));
						bytesRead += sizeof(key);
					}

					anim.AddCluster(jointNames[index], cluster);
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

}