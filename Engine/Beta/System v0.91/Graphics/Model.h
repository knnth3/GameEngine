#pragma once
#include "Mesh.h"

namespace Graphics
{
	//Typedefs
	typedef int32_t TextureID;

	enum class DrawStyle
	{
		POINT_3D = 0,
		POINT_2D,
		LINE_3D,
		LINE_2D,
		TRIANGLE_3D,
		TRIANGLE_2D
	};

	class IDManager
	{
	public:
		static int AssignID();
		static void RemoveID(int id);

	private:
		static std::vector<int> currentIDs;
	};

	class Model
	{
	public:
		//Pre-defined
		GRAPHICS_DLL_API Model();
		GRAPHICS_DLL_API Model(const MeshID id);
		GRAPHICS_DLL_API ~Model();
		GRAPHICS_DLL_API void SetTexture(TextureID id);
		GRAPHICS_DLL_API void SetMesh(MeshID id);
		GRAPHICS_DLL_API void SetDrawStyle(DrawStyle style);
		GRAPHICS_DLL_API TextureID GetTexture();
		GRAPHICS_DLL_API MeshID GetMesh();
		GRAPHICS_DLL_API int GetUniqueID();
		GRAPHICS_DLL_API DrawStyle GetDrawStyle();

		//Set Functions

		GRAPHICS_DLL_API void Scale(const float x, const float y, const float z);
		GRAPHICS_DLL_API void Scale(glm::vec3 scale);
		GRAPHICS_DLL_API void SetPosition(const float x, const float y, const float z);
		GRAPHICS_DLL_API void SetPosition(glm::vec3 pos);
		GRAPHICS_DLL_API void Rotate(float x, float y, float z);
		GRAPHICS_DLL_API void Rotate(glm::vec3 rotation);
		GRAPHICS_DLL_API void RotateAtOrigin(float x, float y, float z);
		GRAPHICS_DLL_API void SetColor(float r, float g, float b);
		GRAPHICS_DLL_API void SetColor(float r, float g, float b, float a);
		GRAPHICS_DLL_API void SetColor(glm::vec4 color);
		GRAPHICS_DLL_API void SetOpacity(float alpha);

		//Get Functions

		GRAPHICS_DLL_API glm::vec3 GetPosition();
		GRAPHICS_DLL_API glm::vec3 GetScale();
		GRAPHICS_DLL_API glm::mat4 GetModelMatrix();
		GRAPHICS_DLL_API glm::vec4 GetColor();

	private:
		void Initialize();
		void CreateLocalToWorld();
		void RotateMatrix(glm::mat4& matrix, glm::vec3 rotations);

		int m_uniqueID;
		MeshID m_meshID;
		TextureID m_textureID;
		DrawStyle m_drawStyle;
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