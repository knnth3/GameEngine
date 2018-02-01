#pragma once
#include <vector>

#define MAX_VERTEX_JOINT_BLENDS 4

template <typename T>
struct Vector3
{
	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(T value)
	{
		x = value;
		y = value;
		z = value;
	}
	bool operator==(const Vector3<T>& v)
	{
		if (this->x == v.x && this->y == v.y && this->z == v.z)
		{
			return true;
		}
		return false;
	}

	T x;
	T y;
	T z;
};

struct Matrix4f
{
	//Identity matrix
	Matrix4f()
	{
		int count = 0;
		for (auto x = 0; x < 4; x++)
		{
			for (auto y = 0; y < 4; y++)
			{
				if (count % 5 == 0)
				{
					Properties[x][y] = 1.0f;
				}
				else
				{
					Properties[x][y] = 0.0f;
				}
				count++;
			}
		}
	}

	float Properties[4][4];
};

struct Joint
{
	std::string Name;
	Matrix4f GlobalBindPoseInverse;
};

struct Skeleton
{
	std::string SkinName;
	std::vector<Joint> Joints;
};

struct JointData
{
	uint32_t ID;
	float Weight;
};

struct VertexData
{
	VertexData()
	{
		bHasUV = false;
		Position = Vector3<float>(0.0f);
		UV = Vector3<float>(0.0f);
		Normal = Vector3<float>(0.0f);
		Tangent = Vector3<float>(0.0f);
		Binormal = Vector3<float>(0.0f);
		Color = Vector3<float>(1.0f);
		
		for (int x = 0; x < MAX_VERTEX_JOINT_BLENDS; x++)
		{
			BlendInfo[x] = {};
		}
	}

	bool AddBlendInfo(JointData data)
	{
		for (int x = 0; x < MAX_VERTEX_JOINT_BLENDS; x++)
		{
			if (BlendInfo[x].Weight == 0.0f)
			{
				BlendInfo[x] = data;
				return true;
			}
		}
		return false;
	}

	bool operator==(const VertexData& v)
	{
		auto pos = this->Position == v.Position;
		auto normals = this->Normal == v.Normal;
		auto tangents = this->Tangent == v.Tangent;
		auto bitangents = this->Binormal == v.Binormal;
		auto textureCoords = this->UV == v.UV;
		auto colors = this->Color == v.Color;

		if (pos && normals && tangents && bitangents && textureCoords && colors)
		{
			return true;
		}
		return false;
	}

	bool bHasUV;
	Vector3<float> Position;
	Vector3<float> UV;
	Vector3<float> Color;
	Vector3<float> Normal;
	Vector3<float> Tangent;
	Vector3<float> Binormal;
	JointData BlendInfo[MAX_VERTEX_JOINT_BLENDS];
};

struct MeshData
{
	bool operator==(const MeshData& data)
	{
		if (Vertices.size() != data.Vertices.size())
			return false;

		if (Indices.size() != data.Indices.size())
			return false;

		for (int it = 0; it < Vertices.size(); it++)
		{
			if (!(this->Vertices[it] == data.Vertices[it]))
			{
				return false;
			}
		}

		for (int it = 0; it < this->Indices.size(); it++)
		{
			if (!(this->Indices[it] == data.Indices[it]))
			{
				return false;
			}
		}

		return true;
	}

	std::string Name;
	std::vector<unsigned int> Indices;
	std::vector<VertexData> Vertices;
	std::vector<bool> IndexedIDs;
};