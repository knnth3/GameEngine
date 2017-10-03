#pragma once
#include <fbxsdk.h>
#include "Primitives.h"


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

		//Forward decls in Model
		struct Vertex;
		class MeshData;


		typedef uint16_t Texture;
		typedef short MeshID;
		typedef const uint32_t c_uint;
		typedef short ModelType;
		typedef std::shared_ptr<MeshData> MeshData_ptr;
		typedef std::shared_ptr<Vertex> Vertex_ptr;

		struct Vertex
		{
			glm::vec3 m_position;
			glm::vec2 m_uv;
			glm::vec3 m_normal;
		};

		//-Class that handles each Polygon face on a mesh
		//-Max Vertices == sizeof(uint32_t)
		class Polygon
		{
			friend class MeshData;
			friend class MeshLoader;
		public:
			AppDLL_API c_uint GetVertexCount();
			AppDLL_API c_uint GetIndexCount();

		protected:
			std::vector<uint32_t> m_indices;
			std::vector<Vertex_ptr> m_vertices;
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
			std::vector<Polygon> polygons;
			uint32_t m_indexCount = 0;
		private:
			std::vector<Vertex_ptr> m_vertices;
		};

		//-This class instantiates any object needed to be rendered to the screen.
		//-Functions of this class modify soely the objects unique attributes
		//-This class requires MeshData_ptr which is by a given MeshID
		//-All functions of this class use Radians for angle measurement
		//and meters for length;
		class Model3D
		{
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
			MeshData_ptr m_mesh;
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
			friend class Model3D;
		public:
			//-Loads a model from a file
			//-Currently only supports fbx files
			//-Only loads the last model in a scene
			//-Optional: takes in the requested model type and builds accordingly
			//so far onyl TEXT has different outcomes
			AppDLL_API static MeshID LoadModel(std::string filename, ModelType type = Model::NONE);

		protected:
			static void GrabMeshData(MeshID id, MeshData_ptr& ptr);
			static void SetDefaultSettings(MeshID id, Model3D& ptr);

		private:
			static bool GrabMeshNormal(FbxMesh* mesh, Vertex& vert, int vertIndex);
			static void ProcessElement(FbxNode* pNode, MeshData_ptr& data, ModelType type);
			static glm::vec3 FbxToGlmVec(FbxDouble3 in);

			std::vector<MeshData_ptr> m_modelLibrary;
			std::vector<MeshDefaultSettings> m_defaultSettings;
		};

	}
}
