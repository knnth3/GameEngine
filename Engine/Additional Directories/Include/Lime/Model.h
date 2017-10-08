#pragma once
#include <fbxsdk.h>
#include "Lime.h"


namespace Lime
{
	//Forward decls in Lime
	class TextInfo;
	class DX11Graphics;
	namespace Model
	{
		//Enum to identify different model uses
		enum
		{
			NONE = -1,
			TEXT = 0,
			MESH = 1
		};

		struct MeshDefaultSettings
		{
			glm::vec3 scale;
			glm::vec3 rotation;
			glm::vec3 translation;
		};

		struct Vertex
		{
			glm::vec3 m_position;
			glm::vec2 m_uv;
			glm::vec3 m_normal;
		};

		struct Vertex2D
		{
			glm::vec2 m_position;
			glm::vec2 m_uv;
			glm::vec4 m_color;
		};

		typedef uint16_t Texture;
		typedef short MeshID;
		typedef const uint32_t c_uint;
		typedef short ModelType;
		typedef std::shared_ptr<Vertex> Vertex_ptr;

		//-Class that handles each Polygon face on a mesh
		//-Max Vertices == sizeof(uint32_t)
		template<class T>
		class Polygon
		{
			friend class MeshData;
			friend class MeshLoader;
		public:
			AppDLL_API c_uint GetVertexCount();
			AppDLL_API c_uint GetIndexCount();

		protected:
			std::vector<uint32_t> m_indices;
			std::vector<T> m_vertices;
		};

		class MeshData
		{
			friend class MeshLoader;
		public:
			AppDLL_API void GetBuffers(std::vector<Vertex>& verts, std::vector<uint32_t>& indices) const;
			AppDLL_API uint32_t GetIndexCount();
			uint32_t objectID = 0;
			uint32_t vertOffset = 0;
			uint32_t indiciOffset = 0;
		protected:
			std::vector<Polygon<Vertex>> m_polygons;
		};

		//-This class instantiates any object needed to be rendered to the screen.
		//-Functions of this class modify soely the objects unique attributes
		//-This class requires MeshData_ptr which is by a given MeshID
		//-All functions of this class use Radians for angle measurement
		//and meters for length;
		class Model3D
		{
			friend class VertexBuffer;
			friend class Lime::DX11Graphics;
			friend class Lime::TextInfo;
		public:
			AppDLL_API Model3D();
			AppDLL_API Model3D(const MeshID id);
			AppDLL_API void Scale(const float x, const float y, const float z);
			AppDLL_API void Scale(glm::vec3 scale);
			AppDLL_API void SetPosition(const float x, const float y, const float z);
			AppDLL_API void SetPosition(glm::vec3 pos);
			AppDLL_API void Rotate(float x, float y, float z);
			AppDLL_API void Rotate(glm::vec3 rotation);
			AppDLL_API void RotateAtOrigin(float x, float y, float z);
			AppDLL_API void SetColor(float r, float g, float b);
			AppDLL_API void SetColor(float r, float g, float b, float a);
			AppDLL_API void SetColor(glm::vec4 color);
			AppDLL_API void SetOpacity(float alpha);
			AppDLL_API void SetOffset(float offset);
			AppDLL_API void SetTexture(Texture tex);
			AppDLL_API void AddMesh(MeshID id);
			AppDLL_API glm::vec3 GetPosition()const;
			AppDLL_API glm::mat4 GetModelMatrix();
			AppDLL_API glm::vec4 GetColor();
			AppDLL_API Texture GetTexture();

			//Operator overloading
			AppDLL_API Model3D& operator= (const Model3D& m) = default;
			AppDLL_API void operator= (MeshID id);


		protected:
			ModelType modelType = NONE;
			std::shared_ptr<MeshData> m_mesh;
			void * m_ptr;
		private:
			void CreateLocalToWorld();
			void RotateMatrix(glm::mat4& matrix, glm::vec3 rotations);
			glm::mat4 m_outRotation;
			glm::mat4 m_inRotation;
			glm::mat4 m_scaleMatrix;
			glm::mat4 m_translation;
			glm::mat4 m_localToWorld;
			glm::vec3 m_position;
			glm::vec3 m_offset;
			glm::vec3 m_scale;
			glm::vec4 m_color;
			uint16_t m_texture;
		};

		class VertexBuffer
		{
		public:
			AppDLL_API bool empty();
			AppDLL_API void clear();
			AppDLL_API size_t size();
			AppDLL_API void AddModel(std::shared_ptr<Model::Model3D>& model);
			AppDLL_API const void* VertexData();
			AppDLL_API const void* IndexData();
			AppDLL_API uint32_t VertexDataSize();
			AppDLL_API uint32_t IndexDataSize();

			//Operator overloads
			AppDLL_API std::shared_ptr<Model::Model3D>& operator[] (const size_t index);

		private:
			int VertCountOffset = 0;
			int IndCountOffset = 0;
			std::vector<UINT> m_cachedIDs;
			std::vector<Vertex> m_vertices;
			std::vector<uint32_t> m_indices;
			std::vector<std::shared_ptr<Model::Model3D>> m_models;
		};

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
