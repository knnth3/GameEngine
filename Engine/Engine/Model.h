#pragma once
#include <glm\glm.hpp>
#include "Primitives.h"


namespace Lime
{
	namespace Model
	{

		typedef uint16_t Texture;
		typedef uint16_t MeshID;
		typedef const uint32_t c_uint;
		typedef std::shared_ptr<MeshData> MeshData_ptr;

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
			friend class MeshLoader;
		public:
			AppDLL_API c_uint GetVertexCount();

		protected:
			uint32_t m_vertexCount;
			std::vector<uint32_t> m_indices;
			std::vector<Vertex> m_vertices;
		};

		struct MeshData
		{
			unsigned int m_ObjectID = 0;
			unsigned int m_VertOffset = 0;
			unsigned int m_IndiciOffset = 0;
			std::vector<Polygon> m_polygons;
		};

		//-This class instantiates any object needed to be rendered to the screen.
		//-Functions of this class modify soely the objects unique attributes
		//-This class requires MeshData_ptr which is by a given MeshID
		//-All functions of this class use Radians for angle measurement
		//and meters for length;
		class Model3D
		{
			AppDLL_API Model3D();
			AppDLL_API Model3D(const MeshID id);
			AppDLL_API void Scale(const float x, const float y, const float z);
			AppDLL_API void SetPosition(const float x, const float y, const float z);
			AppDLL_API void SetPosition(glm::vec3 pos);
			AppDLL_API void Rotate(float x, float y, float z);
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
			AppDLL_API void operator= (const MeshID id);



			std::string renderType = "Mesh";
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

		//-Class used to create mesh objects
		//-Models can be loaded in with the provided functions
		class MeshLoader
		{
			friend class Model3D;
		public:
			AppDLL_API static MeshID LoadModel(std::string filename);

		protected:
			static void GrabMeshData(MeshID id, MeshData_ptr& ptr);

		private:
			static c_uint AssignID();

			static std::vector<MeshData_ptr> m_modelLibrary;
		};


	}
}
