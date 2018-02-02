#pragma once
#include <vector>

#define MAX_VERTEX_JOINT_BLENDS 4

template <typename T>
struct Vector3
{
	T x;
	T y;
	T z;
};

struct Matrix4f
{
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
	std::string Name;
	std::vector<unsigned int> Indices;
	std::vector<VertexData> Vertices;
};

template<typename T>
inline Vector3<T> InitVector3(const T& x, const T& y, const T& z)
{
	Vector3<T> retVal = {};
	retVal.x = x;
	retVal.y = y;
	retVal.z = z;

	return retVal;


}

inline Matrix4f IdentityMatrix4f()
{
	Matrix4f retVal = {};
	int count = 0;
	for (auto x = 0; x < 4; x++)
	{
		for (auto y = 0; y < 4; y++)
		{
			if (count % 5 == 0)
			{
				retVal.Properties[x][y] = 1.0f;
			}
			else
			{
				retVal.Properties[x][y] = 0.0f;
			}
			count++;
		}
	}
	return retVal;
}

inline VertexData InitVertexData()
{
	VertexData retVal = {};
	retVal.bHasUV = false;
	retVal.Position = {};
	retVal.UV = {};
	retVal.Normal = {};
	retVal.Tangent = {};
	retVal.Binormal = {};
	retVal.Color = {};

	for (int x = 0; x < MAX_VERTEX_JOINT_BLENDS; x++)
	{
		retVal.BlendInfo[x] = {};
	}
	return retVal;
}

template<typename T>
inline const bool IsEqual(const Vector3<T>& one, const Vector3<T>& two)
{
	bool result = true;

	if (one.x != two.x)
		result = false;
	else if (one.y != two.y)
		result = false;
	else if (one.z != two.z)
		result = false;

	return result;
}

inline const bool IsEqual(const VertexData & one, const VertexData & two)
{
	bool UVs          = IsEqual(one.UV, two.UV);
	bool colors       = IsEqual(one.Color, two.Color);
	bool normals      = IsEqual(one.Normal, two.Normal);
	bool position     = IsEqual(one.Position, two.Position);
	bool tangents     = IsEqual(one.Tangent, two.Tangent);
	bool bitangents   = IsEqual(one.Binormal, two.Binormal);

	if (position && normals && tangents && bitangents && UVs && colors)
	{
		return true;
	}
	return false;
}

inline const bool IsEqual(const MeshData & one, const MeshData & two)
{
	if (one.Vertices.size() != two.Vertices.size())
		return false;

	if (one.Indices.size() != two.Indices.size())
		return false;

	for (int it = 0; it < one.Vertices.size(); it++)
	{
		if (!IsEqual(one.Vertices[it], two.Vertices[it]))
		{
			return false;
		}
	}

	for (int it = 0; it < one.Indices.size(); it++)
	{
		if (one.Indices[it] != two.Indices[it])
		{
			return false;
		}
	}

	return true;
}

