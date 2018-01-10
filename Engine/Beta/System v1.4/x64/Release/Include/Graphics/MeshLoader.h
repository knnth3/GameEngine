#pragma once
#include "DllSettings.h"
#include "Mesh.h"
#include "fbxsdk.h"

namespace Graphics
{
	struct MeshDefaultSettings
	{
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::vec3 translation;
	};


	//Library that holds given mesh information
	//DEFAULT ID = -1 will load a cube
	class MeshLibrary
	{
		friend class MeshLoader;
	public:
		GRAPHICS_DLL_API MeshLibrary();
		GRAPHICS_DLL_API MeshID SaveMesh(const std::shared_ptr<Mesh>& mesh, const MeshDefaultSettings& setting);
		GRAPHICS_DLL_API void Clear();

		//Returns -1 if not found
		GRAPHICS_DLL_API MeshID IsFilepathQuerried(const std::string filepath);
		GRAPHICS_DLL_API MeshID IsKeyNameQuerried(const std::string filepath);

	protected:
		std::shared_ptr<Mesh> m_default;
		std::vector<std::shared_ptr<Mesh>> m_modelLibrary;
		std::vector<MeshDefaultSettings> m_defaultSettings;
		std::map<std::string, MeshID> m_filepaths;
		std::map<std::string, MeshID> m_keyNames;
	};


	//-Class used to create mesh objects
	//-Models can be loaded in with the provided functions
	class MeshLoader
	{
	public:
		//-Loads a model from a file
		//-Currently only supports fbx files
		//-Only loads the last model in a scene
		//-Model Reqs:
		//--Must be Triangulated
		GRAPHICS_DLL_API static MeshID LoadModel(const std::string& filename);
		GRAPHICS_DLL_API static MeshID LoadModel(const std::vector<Vertex>& verts, const std::vector<Index>& indices, const std::string uniqueName);
		GRAPHICS_DLL_API static MeshID CreateLine(glm::vec3 pos1, glm::vec3 pos2);
		GRAPHICS_DLL_API static void Clear();
		static void GrabMeshData(MeshID id, std::shared_ptr<Mesh>& ptr);
		static void GetDefaulMeshInfo(MeshID id, MeshDefaultSettings& settings);

	private:
		//Mesh saving
		static MeshID SaveInformation(MeshLibrary& library, const std::shared_ptr<Mesh>& data, const MeshDefaultSettings& settings);

		//FBX file processing
		static bool InitFBXObjects(FbxManager*& manager, FbxScene*& scene);
		static bool LoadFBXSceneFromFile(FbxManager* manager, FbxScene* scene, const std::string& filename);
		static void Create3DMeshFromFBX(FbxNode* pNode, std::shared_ptr<Mesh>& data, MeshDefaultSettings& settings);
		static bool GetFBXTextureCoordinates(FbxMesh* mesh, Vertex& vert, int totalIndexCount);
		static bool GetFBXMeshNormals(FbxMesh* mesh, Vertex& vert, int totalIndexCount);

		//General Use
		static glm::vec3 FbxVec4ToGlmVec3(const FbxVector4& in);
	};
}