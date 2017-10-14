#pragma once
#include "Model3D.h"



namespace Lime
{
	namespace Model
	{
		//Library that holds given mesh information
		class MeshLibrary
		{
			friend class MeshLoader;
		public:
			AppDLL_API MeshID SaveMesh(const std::shared_ptr<MeshData>& mesh, const MeshDefaultSettings& setting);
		protected:
			std::vector<std::shared_ptr<MeshData>> m_modelLibrary;
			std::vector<MeshDefaultSettings> m_defaultSettings;
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
			//--Must be Quadrangulated
			AppDLL_API static MeshID LoadModel(const std::string& filename, ModelType type = Model::NONE);
			AppDLL_API static MeshID LoadModel(const std::vector<Vertex>& verts, const std::vector<uint32_t>& indices);

		protected:
			static void GrabMeshData(MeshID id, std::shared_ptr<MeshData>& ptr);
			static void GetDefaulMeshInfo(MeshID id, Model3D& ptr);

		private:
			//Mesh saving
			static MeshID SaveInformation(MeshLibrary& library, const std::shared_ptr<MeshData>& data, const MeshDefaultSettings& settings);

			//FBX file processing
			static bool InitFBXObjects(FbxManager*& manager, FbxScene*& scene);
			static bool LoadFBXSceneFromFile(FbxManager* manager, FbxScene* scene, const std::string& filename);
			static void Create3DMeshFromFBX(FbxNode* pNode, std::shared_ptr<MeshData>& data, MeshDefaultSettings& settings, ModelType type);
			static bool GetFBXTextureCoordinates(FbxMesh* mesh, Vertex& vert, int totalIndexCount);
			static bool GetFBXMeshNormals(FbxMesh* mesh, Vertex& vert, int totalIndexCount);

			//General Use
			static glm::vec3 FbxVec4ToGlmVec3(const FbxVector4& in);
		};
	}
}