#pragma once
#include <vector>


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
};

struct MeshData
{
	MeshData()
	{
		m_indexCount = 0;
		m_vertexCount = 0;
	}

	bool operator==(const MeshData& data)
	{
		if (data.m_vertexCount != this->m_vertexCount)
			return false;

		if (data.m_indexCount != this->m_indexCount)
			return false;

		for (int it = 0; it < this->m_vertexCount; it++)
		{
			if (!(this->m_vertices[it] == data.m_vertices[it]))
			{
				return false;
			}
		}

		for (int it = 0; it < this->m_indexCount; it++)
		{
			if (!(this->m_indices[it] == data.m_indices[it]))
			{
				return false;
			}
		}

		return true;
	}

	int m_indexCount;
	int m_vertexCount;
	std::vector<unsigned int> m_indices;
	std::vector<VertexData> m_vertices;
};