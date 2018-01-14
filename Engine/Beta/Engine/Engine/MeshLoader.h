#pragma once
#include "DllSettings.h"
#include "Mesh.h"

namespace Engine
{
	//-Class used to create mesh objects
	//-Models can be loaded in with the provided functions
	class MeshLoader
	{
	public:

		__engine_decl static bool Initialize();
		__engine_decl static MeshID LoadModel(const std::string filename);
		__engine_decl static MeshID LoadModel(const std::vector<Vertex>& verts, const std::vector<Index>& indices, const std::string uniqueName);
		__engine_decl static MeshID CreatePlane(float xUnits, float zUnits, int xTesselation, int zTesselation);
		__engine_decl static void Clear();
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