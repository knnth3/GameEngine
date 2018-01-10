#pragma once
#include <vector>


template <typename T>
struct Vector3
{
	Vector3()
	{
		x = T();
		y = T();
		z = T();
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
		m_position = Vector3<float>();
		m_uv = Vector3<float>();
		m_color = Vector3<float>();
		m_normal = Vector3<float>();
		m_tangent = Vector3<float>();
		m_binormal = Vector3<float>();
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
		m_indexSize = 0;
		m_vertexSize = 0;
	}

	bool operator==(const MeshData& data)
	{
		if (data.m_vertexSize != this->m_vertexSize)
			return false;

		if (data.m_indexSize != this->m_indexSize)
			return false;

		for (int it = 0; it < this->m_vertexSize; it++)
		{
			if (!(this->m_vertices[it] == data.m_vertices[it]))
			{
				return false;
			}
		}

		for (int it = 0; it < this->m_indexSize; it++)
		{
			if (!(this->m_indices[it] == data.m_indices[it]))
			{
				return false;
			}
		}

		return true;
	}

	int m_indexSize;
	int m_vertexSize;
	std::vector<unsigned int> m_indices;
	std::vector<VertexData> m_vertices;
};