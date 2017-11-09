#pragma once
#include "Model.h"

//Units
//Distance = Meter
//Rotation = Radians
namespace Graphics
{
	namespace Model
	{

		//-This class instantiates any 3D object needed to be rendered to the screen.
		class Model3D
		{
		public:
			DLL_API Model3D();
			DLL_API Model3D(const MeshID id);
			DLL_API ~Model3D();

			//Set Functions

			DLL_API void Scale(const float x, const float y, const float z);
			DLL_API void Scale(glm::vec3 scale);
			DLL_API void SetPosition(const float x, const float y, const float z);
			DLL_API void SetPosition(glm::vec3 pos);
			DLL_API void Rotate(float x, float y, float z);
			DLL_API void Rotate(glm::vec3 rotation);
			DLL_API void RotateAtOrigin(float x, float y, float z);
			DLL_API void SetColor(float r, float g, float b);
			DLL_API void SetColor(float r, float g, float b, float a);
			DLL_API void SetColor(glm::vec4 color);
			DLL_API void SetOpacity(float alpha);
			DLL_API void SetTexture(TextureID tex);
			DLL_API void SetMesh(MeshID id);

			//Get Functions

			DLL_API glm::vec3 GetPosition()const;
			DLL_API glm::vec3 GetScale()const;
			DLL_API glm::mat4 GetModelMatrix();
			DLL_API glm::vec4 GetColor();
			DLL_API TextureID GetTexture();
			DLL_API MeshID GetMesh();
			DLL_API int GetUniqueID();

		private:
			void Initialize();
			void CreateLocalToWorld();
			void RotateMatrix(glm::mat4& matrix, glm::vec3 rotations);

			int m_uniqueID;
			MeshID m_meshID;
			TextureID m_textureID;
			glm::mat4 m_outRotation;
			glm::mat4 m_inRotation;
			glm::mat4 m_scaleMatrix;
			glm::mat4 m_translation;
			glm::mat4 m_localToWorld;
			glm::vec3 m_position;
			glm::vec3 m_scale;
			glm::vec4 m_color;
		};
	}
}