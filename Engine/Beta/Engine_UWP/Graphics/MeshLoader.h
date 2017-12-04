#pragma once
#include "DllSettings.h"
#include "Mesh.h"

namespace Graphics
{
	struct MeshDefaultSettings
	{
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::vec3 translation;
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
		GRAPHICS_DLL_API static MeshID CreateLine(glm::vec3 pos1, glm::vec3 pos2);
		GRAPHICS_DLL_API static void Clear();
		static void GrabMeshData(MeshID id, std::shared_ptr<Mesh>& ptr);
		static void GetDefaulMeshInfo(MeshID id, MeshDefaultSettings& settings);

	private:
		//Mesh saving
		static MeshID SaveInformation(const std::shared_ptr<Mesh>& data, const MeshDefaultSettings& settings);

		//OBJ file processing
		static MeshID ProcessOBJFile(const std::string filename);
		static bool CreateMeshFromOBJ(const std::string filename, std::shared_ptr<Mesh>& data, MeshDefaultSettings & settings);
		static void CalculateTangentBinormal(Polygon& p);
		static void CalculateNormal(Polygon& p);

		//Library usage
		static MeshID SaveMesh(const std::shared_ptr<Mesh>& mesh, const MeshDefaultSettings& setting);
		static bool IsFilepathQuerried(const std::string filepath, MeshID& result);
		static bool IsKeyNameQuerried(const std::string filepath, MeshID& result);

		static bool m_bIsInit;
		static std::shared_ptr<Mesh> m_default;
		static std::vector<std::shared_ptr<Mesh>> m_modelLibrary;
		static std::vector<MeshDefaultSettings> m_defaultSettings;
		static std::map<std::string, MeshID> m_filepaths;
		static std::map<std::string, MeshID> m_keyNames;
	};
}