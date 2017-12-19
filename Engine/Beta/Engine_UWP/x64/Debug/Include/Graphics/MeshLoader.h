#pragma once
#include "DllSettings.h"
#include "Mesh.h"

namespace Graphics
{
	template <typename T>
	struct Vector3
	{
		bool operator==(const Vector3<T>& v);
		glm::vec3 to_glm();

		T x;
		T y;
		T z;
	};

	struct VertexData
	{
		bool operator==(const VertexData& v);

		Vector3<float> m_position;
		Vector3<float> m_uv;
		Vector3<float> m_normal;
		Vector3<float> m_tangent;
		Vector3<float> m_binormal;
	};

	//-Class used to create mesh objects
	//-Models can be loaded in with the provided functions
	class MeshLoader
	{
	public:

		//Model Reqs:
		//	Must be Triangulated

		GRAPHICS_DLL_API static bool Initialize();
		GRAPHICS_DLL_API static MeshID LoadModel(const std::string filename);
		GRAPHICS_DLL_API static MeshID LoadModel(const std::vector<Vertex>& verts, const std::vector<Index>& indices, const std::string uniqueName);
		GRAPHICS_DLL_API static MeshID CreatePlane(float xUnits, float zUnits, int xTesselation, int zTesselation);
		GRAPHICS_DLL_API static void Clear();
		static void GrabMeshData(MeshID id, std::shared_ptr<Mesh>& ptr);

	private:

		static MeshID CreateMesh(const std::string filename);
		static MeshID SaveMesh(const std::shared_ptr<Mesh>& mesh);
		static bool IsFilepathQuerried(const std::string filepath, MeshID& result);
		static bool IsKeyNameQuerried(const std::string filepath, MeshID& result);

		static bool m_bIsInit;
		static std::shared_ptr<Mesh> m_default;
		static std::vector<std::shared_ptr<Mesh>> m_modelLibrary;
		static std::map<std::string, MeshID> m_filepaths;
		static std::map<std::string, MeshID> m_keyNames;
	};
}