#pragma once
#include "Model3D.h"
#include <fbxsdk.h>
#include <map>

namespace Graphics
{
	namespace Model
	{

		struct MeshDefaultSettings
		{
			glm::vec3 scale;
			glm::vec3 rotation;
			glm::vec3 translation;
		};


		//Library that holds given mesh information
		//Default ID = -1 will load a cube
		class MeshLibrary
		{
			friend class MeshLoader;
		public:
			AppDLL_API MeshLibrary();
			AppDLL_API MeshID SaveMesh(const std::shared_ptr<MeshData>& mesh, const MeshDefaultSettings& setting);
			AppDLL_API void Clear();

			//Returns -1 if not found
			AppDLL_API MeshID IsFilepathQuerried(const std::string filepath);
			AppDLL_API MeshID IsKeyNameQuerried(const std::string filepath);

		protected:
			std::shared_ptr<MeshData> m_default;
			std::vector<std::shared_ptr<MeshData>> m_modelLibrary;
			std::vector<MeshDefaultSettings> m_defaultSettings;
			std::map<std::string, MeshID> m_filepaths;
			std::map<std::string, MeshID> m_keyNames;
		};


		//-Class used to create mesh objects
		//-Models can be loaded in with the provided functions
		class MeshLoader
		{
			friend class Model3D;
		public:
			//-Loads a model from a file
			//-Currently only supports fbx files
			//-Only loads the last model in a scene
			//-Model Reqs:
			//--Must be Triangulated
			AppDLL_API static MeshID LoadModel(const std::string& filename);
			AppDLL_API static MeshID LoadModel(const std::vector<Vertex>& verts, const std::vector<uint32_t>& indices, const std::string uniqueName);
			AppDLL_API static MeshID CreateLine(glm::vec3 pos1, glm::vec3 pos2);
			AppDLL_API static void Clear();

		protected:
			static void GrabMeshData(MeshID id, std::shared_ptr<MeshData>& ptr);
			static void GetDefaulMeshInfo(MeshID id, Model3D& ptr);

		private:
			//Mesh saving
			static MeshID SaveInformation(MeshLibrary& library, const std::shared_ptr<MeshData>& data, const MeshDefaultSettings& settings);

			//FBX file processing
			static bool InitFBXObjects(FbxManager*& manager, FbxScene*& scene);
			static bool LoadFBXSceneFromFile(FbxManager* manager, FbxScene* scene, const std::string& filename);
			static void Create3DMeshFromFBX(FbxNode* pNode, std::shared_ptr<MeshData>& data, MeshDefaultSettings& settings);
			static bool GetFBXTextureCoordinates(FbxMesh* mesh, Vertex& vert, int totalIndexCount);
			static bool GetFBXMeshNormals(FbxMesh* mesh, Vertex& vert, int totalIndexCount);

			//General Use
			static glm::vec3 FbxVec4ToGlmVec3(const FbxVector4& in);
		};
	}
}