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
		m_bHasTangents = false;
		m_position = Vector3<float>(0.0f);
		m_uv = Vector3<float>(0.0f);
		m_normal = Vector3<float>(0.0f);
		m_tangent = Vector3<float>(0.0f);
		m_binormal = Vector3<float>(0.0f);
		m_color = Vector3<float>(1.0f);
	}

	bool AddBlendInfo(JointData data)
	{
		if (m_blendInfo.size() < MAX_VERTEX_JOINT_BLENDS)
		{
			m_blendInfo.push_back(data);
			return true;
		}
		return false;
	}

	bool operator==(const VertexData& v)
	{
		auto pos = this->m_position == v.m_position;
		auto normals = this->m_normal == v.m_normal;
		auto tangents = this->m_tangent == v.m_tangent;
		auto bitangents = this->m_binormal == v.m_binormal;
		auto textureCoords = this->m_uv == v.m_uv;
		auto colors = this->m_color == v.m_color;

		if (pos && normals && tangents && bitangents && textureCoords && colors)
		{
			return true;
		}
		return false;
	}

	bool m_bHasTangents;
	Vector3<float> m_position;
	Vector3<float> m_uv;
	Vector3<float> m_color;
	Vector3<float> m_normal;
	Vector3<float> m_tangent;
	Vector3<float> m_binormal;
	std::vector<JointData> m_blendInfo;
};

struct MeshData
{
	bool operator==(const MeshData& data)
	{
		if (m_vertices.size() != data.m_vertices.size())
			return false;

		if (m_indices.size() != data.m_indices.size())
			return false;

		for (int it = 0; it < m_vertices.size(); it++)
		{
			if (!(this->m_vertices[it] == data.m_vertices[it]))
			{
				return false;
			}
		}

		for (int it = 0; it < this->m_indices.size(); it++)
		{
			if (!(this->m_indices[it] == data.m_indices[it]))
			{
				return false;
			}
		}

		return true;
	}

	std::vector<unsigned int> m_indices;
	std::vector<VertexData> m_vertices;
	std::vector<bool> m_indexedIDs;
};