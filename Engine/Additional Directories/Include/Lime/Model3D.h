#pragma once
#include "Model.h"


namespace Lime
{
	namespace Model
	{
		//-This class instantiates any object needed to be rendered to the screen.
		//-Functions of this class modify soely the objects unique attributes
		//-This class requires MeshData_ptr which is by a given MeshID
		//-All functions of this class use Radians for angle measurement
		//and meters for length;
		class Model3D
		{
			friend class VertexLibrary;
			friend class Lime::DX11Graphics;
			friend class Lime::TextInfo;
		public:
			AppDLL_API Model3D();
			AppDLL_API Model3D(const MeshID id);
			AppDLL_API void SetType(Model::MeshType type);
			AppDLL_API void Scale(const float x, const float y, const float z);
			AppDLL_API void Scale(glm::vec3 scale);
			AppDLL_API void SetPosition(const float x, const float y, const float z);
			AppDLL_API void SetPosition(glm::vec3 pos);
			AppDLL_API void Rotate(float x, float y, float z);
			AppDLL_API void Rotate(glm::vec3 rotation);
			AppDLL_API void RotateAtOrigin(float x, float y, float z);
			AppDLL_API void SetColor(float r, float g, float b);
			AppDLL_API void SetColor(float r, float g, float b, float a);
			AppDLL_API void SetColor(glm::vec4 two);
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
			MeshType m_meshType = NONE;
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
	}
}